#include <draw.h>
#include <mouse.h>
#include <cursor.h>
#include "plan9/drawui/data.h"
#include "plan9/drawui/icons.h"

/* colors */
Image *bg_color;
Image *fg_color;
Image *scroll_bg_color;
Image *focus_color;
Image *back_button_focus_color;
Image *forward_button_focus_color;
Image *stop_button_focus_color;
Image *reload_button_focus_color;

/* icons */
Image *back_button_icon;
Image *forward_button_icon;
Image *stop_button_icon;
Image *reload_button_icon;
Image *tick;

/* cursors */
Cursor linkcursor = {
	{0, 0},
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 
	 0xFF, 0xF0, 0xFF, 0xF0, 0xFF, 0xF8, 0xFF, 0xFC, 
	 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFC, 
	 0xF3, 0xF8, 0xF1, 0xF0, 0xE0, 0xE0, 0xC0, 0x40, },
	{0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x06, 0xC0, 0x1C, 
	 0xC0, 0x30, 0xC0, 0x30, 0xC0, 0x38, 0xC0, 0x1C, 
	 0xC0, 0x0E, 0xC0, 0x07, 0xCE, 0x0E, 0xDF, 0x1C, 
	 0xD3, 0xB8, 0xF1, 0xF0, 0xE0, 0xE0, 0xC0, 0x40, }
};
/*
Cursor waitcursor={
	0, 0,
	0x01, 0x80, 0x03, 0xC0, 0x07, 0xE0, 0x07, 0xe0,
	0x07, 0xe0, 0x07, 0xe0, 0x03, 0xc0, 0x0F, 0xF0,
	0x1F, 0xF8, 0x1F, 0xF8, 0x1F, 0xF8, 0x1F, 0xF8,
	0x0F, 0xF0, 0x1F, 0xF8, 0x3F, 0xFC, 0x3F, 0xFC,

	0x01, 0x80, 0x03, 0xC0, 0x07, 0xE0, 0x04, 0x20,
	0x04, 0x20, 0x06, 0x60, 0x02, 0x40, 0x0C, 0x30,
	0x10, 0x08, 0x14, 0x08, 0x14, 0x28, 0x12, 0x28,
	0x0A, 0x50, 0x16, 0x68, 0x20, 0x04, 0x3F, 0xFC,
};
*/
Cursor waitcursor = {
	{0, 0},
	{ 0x80, 0x01, 0x00, 0x00, 0x3e, 0x7c, 0x3f, 0xf4,
	  0x3f, 0xe4, 0x3b, 0xcc, 0x39, 0x9c, 0x1c, 0x38,
	  0x1e, 0x78, 0x3f, 0xfc, 0x3f, 0xfc, 0x3f, 0xfc,
	  0x3f, 0xfc, 0x3e, 0x7c, 0x00, 0x00, 0x80, 0x01,
	},
	{ 0x7f, 0xfe, 0xff, 0xff, 0xc1, 0x83, 0xc0, 0x0b,
	  0xc0, 0x1b, 0xc4, 0x33, 0xc6, 0x63, 0xe3, 0xc7,
	  0xe1, 0x87, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03,
	  0xc0, 0x03, 0xc1, 0x83, 0xff, 0xff, 0x7f, 0xfe,
	},
};


Cursor caretcursor = {
	{-1, 0},
	{ 0x07, 0xc0, 0x04, 0x40, 0x04, 0x40, 0x06, 0xc0,
	  0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80,
	  0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80,
	  0x06, 0xc0, 0x04, 0x40, 0x04, 0x40, 0x07, 0xc0,
	},
	{ 0x00, 0x00, 0x03, 0x80, 0x03, 0x80, 0x01, 0x00,
	  0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
	  0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
	  0x01, 0x00, 0x03, 0x80, 0x03, 0x80, 0x00, 0x00,
	},
};

Cursor crosscursor = {
	{-7, -7},
	{0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0,
	 0x03, 0xC0, 0x03, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF,
	 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xC0, 0x03, 0xC0,
	 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, },
	{0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
	 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x7F, 0xFE,
	 0x7F, 0xFE, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
	 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, }
};

Cursor helpcursor = {
	{-7,-7},
	{0x0f, 0xf0, 0x1f, 0xf8, 0x3f, 0xfc, 0x7f, 0xfe, 
	 0x7c, 0x7e, 0x78, 0x7e, 0x00, 0xfc, 0x01, 0xf8, 
	 0x03, 0xf0, 0x07, 0xe0, 0x07, 0xc0, 0x07, 0xc0, 
	 0x07, 0xc0, 0x07, 0xc0, 0x07, 0xc0, 0x07, 0xc0, },
	{0x00, 0x00, 0x0f, 0xf0, 0x1f, 0xf8, 0x3c, 0x3c, 
	 0x38, 0x1c, 0x00, 0x3c, 0x00, 0x78, 0x00, 0xf0, 
	 0x01, 0xe0, 0x03, 0xc0, 0x03, 0x80, 0x03, 0x80, 
	 0x00, 0x00, 0x03, 0x80, 0x03, 0x80, 0x00, 0x00, }
};

Cursor tl = {
	{-4, -4},
	{0xfe, 0x00, 0x82, 0x00, 0x8c, 0x00, 0x87, 0xff, 
	 0xa0, 0x01, 0xb0, 0x01, 0xd0, 0x01, 0x11, 0xff, 
	 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 
	 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x1f, 0x00, },
	{0x00, 0x00, 0x7c, 0x00, 0x70, 0x00, 0x78, 0x00, 
	 0x5f, 0xfe, 0x4f, 0xfe, 0x0f, 0xfe, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x00, 0x00, }
};

Cursor t = {
	{-7, -8},
	{0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x06, 0xc0, 
	 0x1c, 0x70, 0x10, 0x10, 0x0c, 0x60, 0xfc, 0x7f, 
	 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xff, 0xff, 
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
	 0x03, 0x80, 0x0f, 0xe0, 0x03, 0x80, 0x03, 0x80, 
	 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, }
};

Cursor tr = {
	{-11, -4},
	{0x00, 0x7f, 0x00, 0x41, 0x00, 0x31, 0xff, 0xe1, 
	 0x80, 0x05, 0x80, 0x0d, 0x80, 0x0b, 0xff, 0x88, 
	 0x00, 0x88, 0x0, 0x88, 0x00, 0x88, 0x00, 0x88, 
	 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0xf8, },
	{0x00, 0x00, 0x00, 0x3e, 0x00, 0x0e, 0x00, 0x1e, 
	 0x7f, 0xfa, 0x7f, 0xf2, 0x7f, 0xf0, 0x00, 0x70, 
	 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 
	 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x00, }
};

Cursor r = {
	{-8, -7},
	{0x07, 0xc0, 0x04, 0x40, 0x04, 0x40, 0x04, 0x58, 
	 0x04, 0x68, 0x04, 0x6c, 0x04, 0x06, 0x04, 0x02, 
	 0x04, 0x06, 0x04, 0x6c, 0x04, 0x68, 0x04, 0x58, 
	 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x07, 0xc0, },
	{0x00, 0x00, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 
	 0x03, 0x90, 0x03, 0x90, 0x03, 0xf8, 0x03, 0xfc, 
	 0x03, 0xf8, 0x03, 0x90, 0x03, 0x90, 0x03, 0x80, 
	 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x00, 0x00, }
};

Cursor br = {
	{-11, -11},
	{0x00, 0xf8, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 
	 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 
	 0xff, 0x88, 0x80, 0x0b, 0x80, 0x0d, 0x80, 0x05, 
	 0xff, 0xe1, 0x00, 0x31, 0x00, 0x41, 0x00, 0x7f, },
	{0x00, 0x00, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 
	 0x0, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 
	 0x00, 0x70, 0x7f, 0xf0, 0x7f, 0xf2, 0x7f, 0xfa, 
	 0x00, 0x1e, 0x00, 0x0e, 0x00, 0x3e, 0x00, 0x00, }
};

Cursor b = {
	{-7, -7},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	 0xff, 0xff, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 
	 0xfc, 0x7f, 0x0c, 0x60, 0x10, 0x10, 0x1c, 0x70, 
	 0x06, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, },
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	 0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 
	 0x03, 0x80, 0x03, 0x80, 0x0f, 0xe0, 0x03, 0x80, 
	 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, }
};

Cursor bl = {
	{-4, -11},
	{0x1f, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 
	 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 
	 0x11, 0xff, 0xd0, 0x01, 0xb0, 0x01, 0xa0, 0x01, 
	 0x87, 0xff, 0x8c, 0x00, 0x82, 0x00, 0xfe, 0x00, },
	{0x00, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 
	 0x0e, 0x00, 0x0f, 0xfe, 0x4f, 0xfe, 0x5f, 0xfe, 
	 0x78, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x00, 0x0, }
};

Cursor l = {
	{-7, -7},
	{0x03, 0xe0, 0x02, 0x20, 0x02, 0x20, 0x1a, 0x20, 
	 0x16, 0x20, 0x36, 0x20, 0x60, 0x20, 0x40, 0x20, 
	 0x60, 0x20, 0x36, 0x20, 0x16, 0x20, 0x1a, 0x20, 
	 0x02, 0x20, 0x02, 0x20, 0x02, 0x20, 0x03, 0xe0, },
	{0x00, 0x00, 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 
	 0x09, 0xc0, 0x09, 0xc0, 0x1f, 0xc0, 0x3f, 0xc0, 
	 0x1f, 0xc0, 0x09, 0xc0, 0x09, 0xc0, 0x01, 0xc0, 
	 0x01, 0xc0, 0x01, 0xc0, 0x01, 0xc0, 0x00, 0x00, }
};

Cursor *cornercursors[9] = {
	&tl, &t, &tr,
	&l, 0, &r,
	&bl, &b, &br,
};

static Image* load_icon(Rectangle r, uchar *data, int ndata)
{
	Image *i;
	int n;

	i = allocimage(display, r, RGBA32, 0, DNofill);
	if(i==nil)
		sysfatal("allocimage: %r");
	n = loadimage(i, r, data, ndata);
	if(n<0)
		sysfatal("loadimage: %r");
	return i;
}

#define TICKW 3

static Image* create_tick(void)
{
	Image *t;

	t = allocimage(display, Rect(0, 0, TICKW, font->height), screen->chan, 0, DWhite);
	if(t == nil)
		return 0;
	/* background color */
	draw(t, t->r, display->white, nil, ZP);
	/* vertical line */
	draw(t, Rect(TICKW/2, 0, TICKW/2+1, font->height), display->black, nil, ZP);
	/* box on each end */
	draw(t, Rect(0, 0, TICKW, TICKW), display->black, nil, ZP);
	draw(t, Rect(0, font->height-TICKW, TICKW, font->height), display->black, nil, ZP);
	return t;
}

void data_init(void)
{
	Rectangle iconr;

	bg_color = display->white;
	fg_color = display->black;
	scroll_bg_color = allocimage(display, Rect(0,0,1,1), screen->chan, 1, 0x999999FF);
	focus_color = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DGreygreen);
	back_button_focus_color = focus_color;
	forward_button_focus_color = focus_color;
	stop_button_focus_color = allocimage(display, Rect(0,0,1,1), screen->chan, 1, 0x721c24ff);
	reload_button_focus_color = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DGreyblue);

	iconr = Rect(0, 0, 16, 16);
	back_button_icon = load_icon(iconr, back_button_data, sizeof back_button_data);
	forward_button_icon = load_icon(iconr, fwd_button_data, sizeof fwd_button_data);
	stop_button_icon = load_icon(iconr, stop_button_data, sizeof stop_button_data);
	reload_button_icon = load_icon(iconr, refresh_button_data, sizeof refresh_button_data);
	tick = create_tick();
}
