/*
 * Copyright 2020 Vincent Sanders <vince@netsurf-browser.org>
 *
 * This file is part of NetSurf.
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file
 * content generator for the about scheme testament page
 */

#include <stdbool.h>
#include <stddef.h>

#include "utils/errors.h"
#include "netsurf/inttypes.h"
#include "testament.h"

#include "private.h"
#include "atestament.h"

typedef struct {
	const char *leaf;
	const char *modtype;
} modification_t;

/**
 * Generate the text of an svn testament which represents the current
 * build-tree status
 *
 * \param ctx The fetcher context.
 * \return true if handled false if aborted.
 */
bool fetch_about_testament_handler(struct fetch_about_context *ctx)
{
	nserror res;
	static modification_t modifications[] = WT_MODIFICATIONS;
	int modidx; /* midification index */

	/* content is going to return ok */
	fetch_about_set_http_code(ctx, 200);

	/* content type */
	if (fetch_about_send_header(ctx, "Content-Type: text/plain"))
		goto fetch_about_testament_handler_aborted;

	res = fetch_about_ssenddataf(ctx,
		"# Automatically generated by NetSurf build system\n\n");
	if (res != NSERROR_OK) {
		goto fetch_about_testament_handler_aborted;
	}

	res = fetch_about_ssenddataf(ctx,
#if defined(WT_BRANCHISTRUNK) || defined(WT_BRANCHISMASTER)
			"# This is a *DEVELOPMENT* build from the main line.\n\n"
#elif defined(WT_BRANCHISTAG) && (WT_MODIFIED == 0)
			"# This is a tagged build of NetSurf\n"
#ifdef WT_TAGIS
			"#      The tag used was '" WT_TAGIS "'\n\n"
#else
			"\n"
#endif
#elif defined(WT_NO_SVN) || defined(WT_NO_GIT)
			"# This NetSurf was built outside of our revision "
			"control environment.\n"
			"# This testament is therefore not very useful.\n\n"
#else
			"# This NetSurf was built from a branch (" WT_BRANCHPATH ").\n\n"
#endif
#if defined(CI_BUILD)
			"# This build carries the CI build number '" CI_BUILD "'\n\n"
#endif
			);
	if (res != NSERROR_OK) {
		goto fetch_about_testament_handler_aborted;
	}

	res = fetch_about_ssenddataf(ctx,
		"Built by %s (%s) from %s at revision %s on %s\n\n",
		GECOS, USERNAME, WT_BRANCHPATH, WT_REVID, WT_COMPILEDATE);
	if (res != NSERROR_OK) {
		goto fetch_about_testament_handler_aborted;
	}

	res = fetch_about_ssenddataf(ctx, "Built on %s in %s\n\n", WT_HOSTNAME, WT_ROOT);
	if (res != NSERROR_OK) {
		goto fetch_about_testament_handler_aborted;
	}

	if (WT_MODIFIED > 0) {
		res = fetch_about_ssenddataf(ctx,
				"Working tree has %d modification%s\n\n",
				WT_MODIFIED, WT_MODIFIED == 1 ? "" : "s");
	} else {
		res = fetch_about_ssenddataf(ctx, "Working tree is not modified.\n");
	}
	if (res != NSERROR_OK) {
		goto fetch_about_testament_handler_aborted;
	}

	for (modidx = 0; modidx < WT_MODIFIED; ++modidx) {
		res = fetch_about_ssenddataf(ctx,
				 "  %s  %s\n",
				 modifications[modidx].modtype,
				 modifications[modidx].leaf);
		if (res != NSERROR_OK) {
			goto fetch_about_testament_handler_aborted;
		}
	}

	fetch_about_send_finished(ctx);

	return true;

fetch_about_testament_handler_aborted:
	return false;
}
