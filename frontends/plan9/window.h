#ifndef NETSURF_PLAN9_WINDOW_H
#define NETSURF_PLAN9_WINDOW_H

struct gui_window
{
	struct browser_window *bw;
	struct dwindow *dw;
	Point caret;
	int caret_height;
	struct gui_window *next;
	struct gui_window *prev;
};

extern struct gui_window_table *plan9_window_table;

#endif
