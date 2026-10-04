//main.c

#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "learn.h"
#include "review.h"
#include "set_dictionary.h"
#include "setting.h"

void draw_title(WINDOW *win);
void draw_menu(WINDOW *win);
void draw_dictionary(WINDOW *win, char file_name[15+1]);

int main() {
	setlocale(LC_ALL,"");

	char background_color[20+1] = {'\0'};
	int background_color_length = 20; 

	char language[20+1] = {'\0'};
	int language_length = 20;

	int learn_words_number = 0;
	int review_words_number = 0;

	//config_init read config file
	if (config_read(background_color, background_color_length, language, language_length, &learn_words_number, &review_words_number) == false) {
		puts("Config syntax is invaild\n");
		return 1;
	}

	initscr();
	//curs_set(0);
	start_color();

	use_default_colors();

	if (strcmp(background_color, "red") == 0)
		init_pair(1,-1,COLOR_RED);
	else if (strcmp(background_color, "black") == 0)
		init_pair(1,-1,COLOR_BLACK);
	else if (strcmp(background_color, "blue") == 0)
		init_pair(1,-1,COLOR_BLUE);
	else if (strcmp(background_color, "green") == 0)
		init_pair(1,-1,COLOR_GREEN);
	else if (strcmp(background_color, "yellow") == 0)
		init_pair(1,-1,COLOR_YELLOW);


	bkgd(COLOR_PAIR(1));
	
	noecho();
	cbreak();
	refresh();

	int internal_dictionary_file_path_length = 30;
	char internal_dictionary_file_path[30 + 1] = {'\0'};
	int file_name_length = 15;
	char file_name[15 + 1] = {'\0'};

	const int title_height = 3, title_width = 80, title_starty = 0, title_startx = 25;
	WINDOW *title = create_window(title_height,title_width,title_starty,title_startx);	

	const int dictionary_height = 3, dictionary_width = 50, dictionary_starty = 5, dictionary_startx = 40;
	WINDOW *dictionary = create_window(dictionary_height,dictionary_width,dictionary_starty,dictionary_startx);

	const int menu_height = 9, menu_width = 50, menu_starty = 10, menu_startx = 40;
	WINDOW *menu = create_window(menu_height,menu_width,menu_starty,menu_startx);
	keypad(menu,true);

	if (title != NULL && menu != NULL) {
		int select = 0, index = 1;

		while (1) {
			werase(menu);
			werase(dictionary);

			draw_title(title);
			draw_dictionary(dictionary, file_name);
			draw_menu(menu);

			mvwchgat(menu,index,19,14,A_REVERSE,COLOR_BLACK,NULL);

			int key = wgetch(menu);

			if (key == KEY_DOWN && select >= 0 && select < 3) {
				select++;
				mvwchgat(menu,index,19,14,A_NORMAL,COLOR_BLACK,NULL);
				index += 2;		
				mvwchgat(menu,index,19,14,A_REVERSE,COLOR_BLACK,NULL);
			} else if (key == KEY_UP && select <=4 && select > 0) {
				select--;
				mvwchgat(menu,index,19,14,A_NORMAL,COLOR_BLACK,NULL);
				index -= 2;		
				mvwchgat(menu,index,19,14,A_REVERSE,COLOR_BLACK,NULL);
			} else if (key == '\n' || key == KEY_ENTER) {
				clear();
				refresh();
				
				switch (select) {
					case 0:
						access_learn(internal_dictionary_file_path);
						break;
					case 1:
						access_review();
						break;
					case 2:
						access_set_dictionary(
								internal_dictionary_file_path,
							       	internal_dictionary_file_path_length,
							       	file_name,
							       	file_name_length
								);
						break;
					case 3:
						access_setting(&learn_words_number, &review_words_number);
						break;
				}
			} else if (key == 'q') {
				break;
			}
		}
	}	

	delwin(title);
	delwin(dictionary);
	delwin(menu);

	endwin();

	return 0;
}

void draw_dictionary(WINDOW *win, char file_name[15+1]) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,3,"You're learning:");		
	mvwprintw(win,1,20,"%s", file_name);
	wrefresh(win);
}

void draw_title(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,29,"Learning English words");
	wrefresh(win);
}

void draw_menu(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,19,"Learn");
	mvwaddstr(win,3,19,"Review");
	mvwaddstr(win,5,19,"Set dictionary");
	mvwaddstr(win,7,19,"Setting");
	wrefresh(win);
}
