//review.c

#include <ncurses.h>

#include "common.h"
#include "review.h"

void draw_review_title(WINDOW *win) {
	if (win == NULL) return;	

	box(win,0,0);
	mvwaddstr(win,1,38,"review");
	wrefresh(win);
}

void draw_display_review_number(WINDOW *win) {
	if (win == NULL) return;	

	box(win,0,0);
	waddstr(win, "remains");
	wrefresh(win);
}

void draw_review_zone(WINDOW *win) {
	if (win == NULL) return;	

	box(win,0,0);
	waddstr(win, "review zone");
	wrefresh(win);
}

void access_review() {
	const int review_title_height = 3, review_title_width = 80, review_title_starty = 0, review_title_startx = 25;
	WINDOW *review_title = create_window(review_title_height, review_title_width, review_title_starty, review_title_startx);

	const int display_number_height = 3, display_number_width = 10, display_number_starty = 5, display_number_startx = 25;	
	WINDOW *display_number = create_window(display_number_height, display_number_width, display_number_starty, display_number_startx);

	const int review_zone_height = 20, review_zone_width = 80, review_zone_starty = 10, review_zone_startx = 25;
	WINDOW *review_zone = create_window(review_zone_height, review_zone_width, review_zone_starty, review_zone_startx);

	if (review_title != NULL) {
		draw_review_title(review_title);	
		draw_display_review_number(display_number);
		draw_review_zone(review_zone);

		getch();
	}

	clear();
	refresh();

	delwin(review_title);
}
