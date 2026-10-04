//set_dictionary.c

#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "set_dictionary.h"
#include "common.h"

static int set_dictionary_file(	const char *dictionary_file_path[4],
				const char *dictionary[4],
				char *target_file_path,
				const int target_file_path_length,
				char *target_file_name,
				const int target_file_name_length,
				const int select,
				const int length )
{
	if (select >= 0 && select < length) {
		snprintf(target_file_path, target_file_path_length, "%s", dictionary_file_path[select]);
		snprintf(target_file_name, target_file_name_length, "%s", dictionary[select]);
		return 0;
	}
	
	return EOF;	
}

void draw_set_dictionary_title(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,20,"Set dictionary");
	wrefresh(win);
}

void draw_set_dictionary_list_menu(WINDOW *win, const char *dictionary[4]) {
	if (win == NULL) return;

	int index = 1;

	box(win,0,0);
	for (int i = 0; i < 4; i++) {
		mvwaddstr(win,index,10,dictionary[i]);
		index += 2;
	}

	wrefresh(win);
}

void access_set_dictionary(char *source_file_path,
			   const int source_file_path_length,
			   char *source_file_name,
			   const int source_file_name_length)
{
	const char *dictionary[4] = {
		"dictionary1",
		"dictionary2",
		"dictionary3",
		"dictionary4"
	};


	int dictionary_file_path_length = 4;
	const char *dictionary_file_path[4] = {
		"../dictionary/dictionary1.txt",
		"../dictionary/dictionary2.txt",
		"../dictionary/dictionary3.txt",
		"../dictionary/dictionary4.txt"
	};

	const int set_dictionary_title_height = 3, set_dictionary_title_width = 50, set_dictionary_starty = 1, set_dictionary_startx = 50;
	WINDOW *set_dictionary_title = create_window(set_dictionary_title_height,set_dictionary_title_width,set_dictionary_starty,set_dictionary_startx);	

	const int set_dictionary_list_menu_height = 10, set_dictionary_list_menu_width = 50, set_dictionary_list_menu_starty = 5, set_dictionary_list_menu_startx = 50;
	WINDOW *set_dictionary_list_menu = create_window(set_dictionary_list_menu_height, set_dictionary_list_menu_width, set_dictionary_list_menu_starty, set_dictionary_list_menu_startx);
	keypad(set_dictionary_list_menu,true);

	if (set_dictionary_title != NULL && set_dictionary_list_menu) {
		int set_dictionary_select = 0, set_dictionary_index = 1;

		while (1) {
			draw_set_dictionary_title(set_dictionary_title);	
			draw_set_dictionary_list_menu(set_dictionary_list_menu,dictionary);

			mvwchgat(set_dictionary_list_menu, set_dictionary_index, 10, 11, A_REVERSE, COLOR_BLACK, NULL);

			int key = wgetch(set_dictionary_list_menu);

			if (key == KEY_DOWN && set_dictionary_select >= 0 && set_dictionary_select < 3) {
				set_dictionary_select++;
				mvwchgat(set_dictionary_list_menu, set_dictionary_index, 10, 11, A_REVERSE, COLOR_BLACK, NULL);
				set_dictionary_index += 2;
				mvwchgat(set_dictionary_list_menu, set_dictionary_index, 10, 11, A_REVERSE, COLOR_BLACK, NULL);
			} else if (key == KEY_UP && set_dictionary_select <= 3 && set_dictionary_select > 0) {
				set_dictionary_select--;
				mvwchgat(set_dictionary_list_menu, set_dictionary_index, 10, 11, A_REVERSE, COLOR_BLACK, NULL);
				set_dictionary_index -= 2;
				mvwchgat(set_dictionary_list_menu, set_dictionary_index, 10, 11, A_REVERSE, COLOR_BLACK, NULL);
			} else if (key == '\n' || key == KEY_ENTER) {
				if (set_dictionary_file(dictionary_file_path, dictionary, source_file_path, source_file_path_length, source_file_name, source_file_name_length, set_dictionary_select, 4) != EOF)
					break;
			} else if (key == 'q') {
				break;
			}
		}
	}

	clear();
	refresh();

	delwin(set_dictionary_title);
	delwin(set_dictionary_list_menu);
}
