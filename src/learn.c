//learn.c

#include <ncurses.h>
#include <stdlib.h>

#include "common.h"
#include "learn.h"

bool read_dictionary(const char *dictionary_file_path, char content[20][100], int content_index) {
	if (dictionary_file_path == NULL)
		return false;

	FILE *read_content = fopen(dictionary_file_path, "r");
	if (read_content == NULL)
		return false;
		
	while (fgets(content[content_index], sizeof(content[content_index]), read_content))
		content_index++;
	
	fclose(read_content);

	return true;
}

static void draw_learn_title(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,35,"learning");
	wrefresh(win);
}

static void draw_display_learn_number(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	waddstr(win,"remains");
	wrefresh(win);
}

static void draw_learn_zone(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,0,0,"learn zone");
	wrefresh(win);
}

void access_learn(const char *dictionary_file_path) {
	const int learn_title_height = 3, learn_title_width = 80, learn_title_starty = 0, learn_title_startx = 25;
	WINDOW *learn_title = create_window(learn_title_height, learn_title_width, learn_title_starty, learn_title_startx);
	const int display_number_height = 3, display_number_width = 10, display_number_starty = 5, display_number_startx = 25;
	WINDOW *display_number = create_window(display_number_height, display_number_width, display_number_starty, display_number_startx);
	
	const int learn_zone_height = 20, learn_zone_width = 80, learn_zone_starty = 10, learn_zone_startx = 25;
	WINDOW *learn_zone = create_window(learn_zone_height, learn_zone_width, learn_zone_starty, learn_zone_startx);
	
	char content[20][100] = {0};
	int content_index = 0;
	if (!read_dictionary(dictionary_file_path, content, content_index))
		return;
	char words[20][100] = {0};
	int words_index = 0;
	char meaning[20][100] = {0};
	int meaning_index = 0;
	if (!split_dictionary(content, words, words_index, meaning, meaning_index))
		return;

	char quiz_word[100] = {0};
	char quiz[4][100] = {0};

	if (learn_title != NULL) {
		draw_learn_title(learn_title);
		draw_display_learn_number(display_number);

		int select = 0;
		while (1) {
			draw_learn_zone(learn_zone);

			/*
						
			//options
			mvwprintw(learn_zone, 5, 5, "1 %s", quiz[0]);
			mvwprintw(learn_zone, 8, 5, "2 %s", quiz[1]);
			mvwprintw(learn_zone, 11, 5, "3 %s", quiz[2]);
			mvwprintw(learn_zone, 14, 5, "4 %s", quiz[3]);
			*/

			int random_word = rand() % 4;
			//word
			mvwprintw(learn_zone, 2, 2, "%s", words[random_word]);
			
			

			wrefresh(learn_zone);

			int key = wgetch(learn_zone);

			if (key == '1') {
				select = key - '1'; 
			} else if (key == '2') {
				select = key - '1';
			} else if (key == '3') {
				select = key - '1';
			} else if (key == '4') {
				select = key - '1'; 
			} else if (key == 'q') {
				break;
			}
			
			printw("%s", meaning[select]);
			refresh();
		}
	}

	clear();
	refresh();

	delwin(learn_title);
	delwin(display_number);
	delwin(learn_zone);
}
