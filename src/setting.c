//setting.c

#include "setting.h"
#include "common.h"

#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <locale.h>

const char *background_colors[6] = {"red", "black", "blue", "green", "yellow"};
const char *language[2] = {"zh_CH", "en_US"};

static void operation_background_color_panel(WINDOW *win) {
	keypad(win, true);
	int select = 0, index = 2;

	while (1) {
		mvwchgat(win,index,5,6,A_REVERSE,COLOR_BLACK,NULL);

		update_panels();
		doupdate();

		int key = wgetch(win);

		if (key == KEY_DOWN && select >= 0 && select < 5) {
			select++;
			mvwchgat(win,index,5,6,A_NORMAL,COLOR_BLACK,NULL);
			index++;
			mvwchgat(win,index,5,6,A_REVERSE,COLOR_BLACK,NULL);	
		} else if (key == KEY_UP && select <= 5 && select > 0) {
			select--;
			mvwchgat(win,index,5,6,A_NORMAL,COLOR_BLACK,NULL);
			index--;
			mvwchgat(win,index,5,6,A_REVERSE,COLOR_BLACK,NULL);
		} else if (key == '\n' || key == KEY_ENTER) {
			config_write("background_color",background_colors[select], 's');
			break;
		} else if (key == 'q') {
			break;
		}
	}
}

static void operation_language_panel(WINDOW *win) {
	keypad(win, true);
	int select = 0, index = 2;

	while (1) {
		mvwchgat(win,index,5,6,A_REVERSE,COLOR_BLACK,NULL);

		update_panels();
		doupdate();

		int key = wgetch(win);

		if (key == KEY_DOWN && select >= 0 && select < 2) {
			select++;
			mvwchgat(win,index,5,6,A_NORMAL,COLOR_BLACK,NULL);
			index++;
			mvwchgat(win,index,5,6,A_REVERSE,COLOR_BLACK,NULL);	
		} else if (key == KEY_UP && select <= 2 && select > 0) {
			select--;
			mvwchgat(win,index,5,6,A_NORMAL,COLOR_BLACK,NULL);
			index--;
			mvwchgat(win,index,5,6,A_REVERSE,COLOR_BLACK,NULL);
		} else if (key == '\n' || key == KEY_ENTER) {
			config_write("language",language[select], 's');
			break;
		} else if (key == 'q') {
			break;
		}
	}
}

static void operation_change_learning_setting_panel(int **learn_words_number, int **review_words_number, WINDOW *win) {
	keypad(win, true);
	int select = 0, index = 2;

	while (1) {
		mvwchgat(win,index,19,2,A_REVERSE,COLOR_BLACK,NULL);

		update_panels();
		doupdate();

		int key = wgetch(win);

		if (key == KEY_DOWN && select >= 0 && select < 1) {
			select++;
			mvwchgat(win,index,19,2,A_NORMAL,COLOR_BLACK,NULL);
			index++;
			mvwchgat(win,index,19,2,A_REVERSE,COLOR_BLACK,NULL);	
		} else if (key == KEY_UP && select <= 1 && select > 0) {
			select--;
			mvwchgat(win,index,19,2,A_NORMAL,COLOR_BLACK,NULL);
			index--;
			mvwchgat(win,index,19,2,A_REVERSE,COLOR_BLACK,NULL);
		} else if (key == '\n' || key == KEY_ENTER) {
			mvwaddch(win,index,19,' ');
			mvwaddch(win,index,20,' ');
			echo();

			char number[2] = {'\0'};
			wgetnstr(win, number, sizeof(number));
			int convert = (int)strtol(number, NULL, 10);

			if (select == 0) {
				**learn_words_number = convert;
				config_write("learn_words_number", &convert, 'n');
			} else {
				**review_words_number = convert;
				config_write("review_words_number", &convert, 'n');
			}
			
			erase();			
			noecho();
		} else if (key == 'q') {
			break;
		}
	}
}

static void draw_and_hide_change_language_panel(PANEL *panel, WINDOW *win, char status) {
	if (panel == NULL || win == NULL) return;
		
	box(win, 0, 0);
	mvwaddstr(win,1,5,"select language");
	mvwchgat(win,1,5,15,A_UNDERLINE,COLOR_BLACK,NULL);

	int index = 2;
	for (int i = 0; i < 2; i++) {
		mvwaddstr(win,index,5,language[i]);
		index++;
	}
	mvwaddstr(win,2,5,"zh_CN");
	mvwaddstr(win,3,5,"en_US");

	if (status == 's')
		show_panel(panel);
	else if (status == 'h')
		hide_panel(panel);
	else
		return;

	update_panels();
	doupdate();
}

static void draw_and_hide_change_background_color_panel(PANEL *panel, WINDOW *win, char status) {
	if (panel == NULL || win == NULL) return;

	box(win, 0, 0);
	mvwaddstr(win,1,5,"select color");
	mvwchgat(win,1,5,15,A_UNDERLINE,COLOR_BLACK,NULL);

	int index = 2;
	for (int i = 0; i < 6; i++) {
		mvwaddstr(win,index,5,background_colors[i]);
		index++;
	}

	if (status == 's')
		show_panel(panel);
	else if (status == 'h')
		hide_panel(panel);
	else
		return;

	update_panels();
	doupdate();
}

static void draw_and_hide_change_learning_setting_panel(const int *learn_words_number, const int *review_words_number, PANEL *panel, WINDOW *win, char status) {
	if (panel == NULL || win == NULL) return;
	
	werase(win);
	
	box(win, 0, 0);
	mvwaddstr(win,1,5,"learning setting");
	mvwchgat(win,1,5,15,A_UNDERLINE,COLOR_BLACK,NULL);

	mvwprintw(win,2,5,"learn words:  %d", *learn_words_number);
	mvwprintw(win,3,5,"review words: %d", *review_words_number);

	if (status == 's')
		show_panel(panel);
	else if (status == 'h')
		hide_panel(panel);
	else
		return;

	update_panels();
	doupdate();
}

static void draw_setting_title(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,29,"Setting");
	wrefresh(win);	
}

static void draw_setting_menu_panel(PANEL *panel, WINDOW *win) {
	if (win == NULL || panel == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,20,"language");
	mvwaddstr(win,3,20,"learning setting");
	mvwaddstr(win,5,20,"background color");

	update_panels();
	doupdate();
}

void access_setting(int *learn_words_number, int *review_words_number) {
	setlocale(LC_ALL, "");

	const int setting_title_height = 3, setting_title_width = 80, setting_title_starty = 0, setting_title_startx = 25;
	WINDOW *setting_title = create_window(setting_title_height,setting_title_width,setting_title_starty,setting_title_startx);
	
	const int setting_menu_height = 12, setting_menu_width = 50, setting_menu_starty = 5, setting_menu_startx = 40;
	WINDOW *setting_menu_window = create_window(setting_menu_height,setting_menu_width,setting_menu_starty,setting_menu_startx);
	PANEL *setting_menu = create_panel(setting_menu_window);
	keypad(setting_menu_window,true);

	const int learning_setting_height = 12, learning_setting_width = 30, learning_setting_starty= 5, learning_setting_startx= 50;
	WINDOW *learning_setting_window = create_window(learning_setting_height,learning_setting_width,learning_setting_starty,learning_setting_startx);
	PANEL *learning_setting = create_panel(learning_setting_window);

	const int language_height = 12, language_width = 30, language_starty = 5, language_startx = 50;
	WINDOW *language_window = create_window(language_height,language_width,language_starty,language_startx);
	PANEL *language = create_panel(language_window);

	const int background_color_height = 12, background_color_width = 30, background_color_starty = 5, background_color_startx = 50;
	WINDOW *background_color_window = create_window(background_color_height,background_color_width,background_color_starty,background_color_startx);
	PANEL *background_color = create_panel(background_color_window);

	if (setting_title != NULL && setting_menu != NULL) {
		int setting_select = 0, setting_index = 1;

		while (1) {	
			draw_setting_title(setting_title);
			draw_setting_menu_panel(setting_menu, setting_menu_window);
			
			draw_and_hide_change_language_panel(language, language_window, 'h');
			draw_and_hide_change_background_color_panel(background_color, background_color_window, 'h');
			draw_and_hide_change_learning_setting_panel(learn_words_number, review_words_number, learning_setting, learning_setting_window, 'h');
		
			mvwchgat(setting_menu_window,setting_index,20,16,A_REVERSE,COLOR_BLACK,NULL);

			int key = wgetch(setting_menu_window);

			if (key == KEY_DOWN && setting_select >= 0 && setting_select < 2) {
				setting_select++;
				mvwchgat(setting_menu_window,setting_index,20,16,A_NORMAL,COLOR_BLACK,NULL);
				setting_index += 2;
				mvwchgat(setting_menu_window,setting_index,20,16,A_REVERSE,COLOR_BLACK,NULL);	
			} else if (key == KEY_UP && setting_select <= 2 && setting_select > 0) {
				setting_select--;
				mvwchgat(setting_menu_window,setting_index,20,16,A_NORMAL,COLOR_BLACK,NULL);
				setting_index -= 2;
				mvwchgat(setting_menu_window,setting_index,20,16,A_REVERSE,COLOR_BLACK,NULL);
			} else if (key == '\n' || key == KEY_ENTER) {
				switch(setting_select) {
					case 0 :
						draw_and_hide_change_language_panel(language, language_window, 's');
						operation_language_panel(language_window);
						break;
					case 1:
						draw_and_hide_change_learning_setting_panel(learn_words_number, review_words_number,learning_setting, learning_setting_window, 's');
						operation_change_learning_setting_panel(&learn_words_number, &review_words_number, learning_setting_window);
						break;
					case 2:
						draw_and_hide_change_background_color_panel(background_color, background_color_window, 's');
						operation_background_color_panel(background_color_window);
						break;
				}
			} else if (key == 'q') {
				break;
			}
		}
	}

	clear();
	refresh();

	del_panel(learning_setting);
	del_panel(language);
	del_panel(background_color);
	del_panel(setting_menu);

	delwin(setting_title);
}
