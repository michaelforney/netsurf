/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2003 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2004 Richard Wilson <not_ginger_matt@hotmail.com>
 */

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <swis.h>
#include "ifc.h"
#include "libpng/png.h"
#include "oslib/colourtrans.h"
#include "oslib/os.h"
#include "oslib/osspriteop.h"
#include "netsurf/utils/config.h"
#include "netsurf/content/content.h"
#include "netsurf/riscos/options.h"
#include "netsurf/riscos/png.h"
#include "netsurf/riscos/tinct.h"
#include "netsurf/utils/log.h"
#include "netsurf/utils/messages.h"
#include "netsurf/utils/utils.h"

#ifdef WITH_PNG
/* libpng uses names starting png_, so use nspng_ here to avoid clashes */

static void info_callback(png_structp png, png_infop info);
static void row_callback(png_structp png, png_bytep new_row,
		png_uint_32 row_num, int pass);
static void end_callback(png_structp png, png_infop info);


void nspng_init(void)
{
}


void nspng_create(struct content *c, const char *params[])
{
	c->data.png.sprite_area = 0;
	c->data.png.png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			0, 0, 0);
	assert(c->data.png.png != 0);
	c->data.png.info = png_create_info_struct(c->data.png.png);
	assert(c->data.png.info != 0);

	if (setjmp(png_jmpbuf(c->data.png.png))) {
		png_destroy_read_struct(&c->data.png.png,
				&c->data.png.info, 0);
		assert(0);
	}

	png_set_progressive_read_fn(c->data.png.png, c,
			info_callback, row_callback, end_callback);
}


void nspng_process_data(struct content *c, char *data, unsigned long size)
{
	if (setjmp(png_jmpbuf(c->data.png.png))) {
		png_destroy_read_struct(&c->data.png.png,
				&c->data.png.info, 0);
		assert(0);
	}

	LOG(("data %p, size %li", data, size));
	png_process_data(c->data.png.png, c->data.png.info,
			data, size);

	c->size += size;
}


/**
 * info_callback -- PNG header has been completely received, prepare to process
 * image data
 */

void info_callback(png_structp png, png_infop info)
{
	int i, bit_depth, color_type, interlace;
	unsigned int rowbytes, sprite_size;
	unsigned long width, height;
	struct content *c = png_get_progressive_ptr(png);
	osspriteop_area *sprite_area;
	osspriteop_header *sprite;
	png_color_16 *png_background;
	png_color_16 default_background = {0, 0xffff, 0xffff, 0xffff, 0xffff};

	/*	Read the PNG details
	*/
	png_get_IHDR(png, info, &width, &height, &bit_depth,
			&color_type, &interlace, 0, 0);

	/*	Claim the required memory for the converted PNG
	*/
	sprite_size = sizeof(*sprite_area) + sizeof(*sprite) + (height * width * 4);
	sprite_area = xcalloc(sprite_size, 1);

	/*	Fill in the sprite area header information
	*/
	sprite_area->size = sprite_size;
	sprite_area->sprite_count = 1;
	sprite_area->first = sizeof(*sprite_area);
	sprite_area->used = sprite_size;

	/*	Fill in the sprite header information
	*/
	sprite = (osspriteop_header *) (sprite_area + 1);
	sprite->size = sprite_size - sizeof(*sprite_area);
	strcpy(sprite->name, "png");
	sprite->width = width - 1;
	sprite->height = height - 1;
	sprite->left_bit = 0;
	sprite->right_bit = 31;
	sprite->mask = sprite->image = sizeof(*sprite);
	sprite->mode = 0x301680b5;

	/*	Store the sprite area
	*/
	c->data.png.sprite_area = sprite_area;

	/*	Set up our transformations
	*/
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_gray_1_2_4_to_8(png);
	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);
	if (bit_depth == 16)
		png_set_strip_16(png);
	if (color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);
	if (!(color_type & PNG_COLOR_MASK_ALPHA))
		png_set_filler(png, 0xff, PNG_FILLER_AFTER);

	png_read_update_info(png, info);

	c->data.png.rowbytes = rowbytes = png_get_rowbytes(png, info);
	c->data.png.interlace = (interlace == PNG_INTERLACE_ADAM7);
	c->data.png.sprite_image = ((char *) sprite) + sprite->image;
	c->width = width;
	c->height = height;

	LOG(("size %li * %li, bpp %i, rowbytes %u", width,
				height, bit_depth, rowbytes));
}


static unsigned int interlace_start[8] = {0, 16, 0, 8, 0, 4, 0};
static unsigned int interlace_step[8] = {28, 28, 12, 12, 4, 4, 0};
static unsigned int interlace_row_start[8] = {0, 0, 4, 0, 2, 0, 1};
static unsigned int interlace_row_step[8] = {8, 8, 8, 4, 4, 2, 2};

void row_callback(png_structp png, png_bytep new_row,
		png_uint_32 row_num, int pass)
{
	struct content *c = png_get_progressive_ptr(png);
	unsigned long i, j, rowbytes = c->data.png.rowbytes;
	unsigned int start, step;
	char *row = c->data.png.sprite_image + row_num * (c->width * 4);

	/*	Abort if we've not got any data
	*/
	if (new_row == 0)
		return;

	/*	Handle interlaced sprites using the Adam7 algorithm
	*/
	if (c->data.png.interlace) {
		start = interlace_start[pass];
 		step = interlace_step[pass];
		row_num = interlace_row_start[pass] +
				interlace_row_step[pass] * row_num;

		/*	Copy the data to our current row taking into consideration interlacing
		*/
		row = c->data.png.sprite_image + row_num * (c->width * 4);
		for (j = 0, i = start; i < rowbytes; i += step) {
			row[i++] = new_row[j++];
			row[i++] = new_row[j++];
			row[i++] = new_row[j++];
			row[i++] = new_row[j++];
		}
	} else {
		/*	Do a fast memcpy of the row data
		*/
		memcpy(row, new_row, rowbytes);
	}
}


void end_callback(png_structp png, png_infop info)
{
	/*struct content *c = png_get_progressive_ptr(png);*/

	LOG(("PNG end"));

	/*xosspriteop_save_sprite_file(osspriteop_USER_AREA, c->data.png.sprite_area,
			"png");*/
}



int nspng_convert(struct content *c, unsigned int width, unsigned int height)
{
	png_destroy_read_struct(&c->data.png.png, &c->data.png.info, 0);

	c->title = xcalloc(100, 1);
	sprintf(c->title, messages_get("PNGTitle"), c->width, c->height);
	c->status = CONTENT_STATUS_DONE;
	return 0;
}


void nspng_revive(struct content *c, unsigned int width, unsigned int height)
{
}


void nspng_reformat(struct content *c, unsigned int width, unsigned int height)
{
}


void nspng_destroy(struct content *c)
{
	xfree(c->title);
	xfree(c->data.png.sprite_area);
}


void nspng_redraw(struct content *c, long x, long y,
		unsigned long width, unsigned long height,
		long clip_x0, long clip_y0, long clip_x1, long clip_y1,
		float scale)
{

	/*	Tinct currently only handles 32bpp sprites that have an embedded alpha mask. Any
		sprites not matching the required specifications are ignored. See the Tinct
		documentation for further information.
	*/
	_swix(Tinct_PlotScaledAlpha, _IN(2) | _IN(3) | _IN(4) | _IN(5) | _IN(6) | _IN(7),
			((char *) c->data.png.sprite_area + c->data.png.sprite_area->first),
			x, (int)(y - height),
			width / 2, height / 2,	// [hack until Tinct is fixed] - rjw
			(option_filter_sprites?0:(1<<1)) | (option_dither_sprites?0:(1<<2)));
}
#endif
