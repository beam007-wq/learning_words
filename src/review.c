//review.c

#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "review.h"

static void draw_review_title(WINDOW *win) {
	if (win == NULL) return;

	box(win,0,0);
	mvwaddstr(win,1,35,"learning");
	wrefresh(win);
}

static void draw_display_review_number(WINDOW *win, int number) {
	if (win == NULL) return;

	box(win,0,0);
	waddstr(win,"remains");
	mvwprintw(win,1,0, "%d", number);
	wrefresh(win);
}

static void draw_review_zone(WINDOW *win, Word *w, char quiz[4][100], Content *meaning) {
	if (win == NULL && w == NULL) return;

	box(win,0,0);
	mvwaddstr(win,0,0,"learn zone");
	mvwprintw(win,2,10,"%d", w->correct_times - 1);

	random_option(w->meaning, meaning, quiz);
	//word
	mvwprintw(win, 2, 2, "%s", w->word);

	//options
	mvwprintw(win, 5, 5, "1 %s", quiz[0]);
	mvwprintw(win, 8, 5, "2 %s", quiz[1]);
	mvwprintw(win, 11, 5, "3 %s", quiz[2]);
	mvwprintw(win, 14, 5, "4 %s", quiz[3]);

	wrefresh(win);
}

static bool check_option(const Word *word, const int select, const char quiz[4][100]) {
    if (word == NULL)
        return false;

    if (strcmp(quiz[select], word->meaning) == 0)
        return true;
    else
        return false;
}

void access_review(const int review_word_number) {
	const int review_title_height = 3, review_title_width = 80, review_title_starty = 0, review_title_startx = 25;
	WINDOW *review_title = create_window(review_title_height, review_title_width, review_title_starty, review_title_startx);
	const int display_number_height = 3, display_number_width = 10, display_number_starty = 5, display_number_startx = 25;
	WINDOW *display_number = create_window(display_number_height, display_number_width, display_number_starty, display_number_startx);

	const int review_zone_height = 20, review_zone_width = 80, review_zone_starty = 10, review_zone_startx = 25;
	WINDOW *review_zone = create_window(review_zone_height, review_zone_width, review_zone_starty, review_zone_startx);

	Content *content = get_file_content("../cache/review.txt");
	if (content == NULL)
		return;

	Content words = {
		.column_index = 0,
		.row_index = 0,
		.column_size = 5,
		.row_size = 1,
	};
	words.content = (char *)malloc(words.row_size * words.column_size);
	if (words.content == NULL)
		return;

	Content meaning = {
		.column_index = 0,
		.row_index = 0,
		.column_size = 5,
		.row_size = 1,
	};
	meaning.content = (char *)malloc(meaning.row_size * meaning.column_size);
	if (meaning.content == NULL)
		return;
	
	if (!split_dictionary(content, &words, &meaning))
		return;
	
	if (review_title != NULL && display_number != NULL && review_zone != NULL) {

		draw_review_title(review_title);

		char quiz[4][100] = {0};
		int select = 0;
		int word_number = review_word_number;

		Word *word = load_words(review_word_number, &words, &meaning);
		if (word == NULL)
		    return;

		while (1) {
			werase(review_zone);
			werase(display_number);

			if (word == NULL)
			    break;

			draw_display_review_number(display_number, word_number);
			draw_review_zone(review_zone, word, quiz, &meaning);

			if (word->correct_times > 2) {
			    word = remove_node(word, word->id);
				word_number--;
			}

			int key = wgetch(review_zone);

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

			if (check_option(word, select, quiz)) {
				word->correct_times += 1;
			    word = word->next;
			}
		}

		if (word!=NULL)
		    free_link_list(word);
	}

	clear();
	refresh();

	delwin(review_title);
	delwin(display_number);
	delwin(review_zone);
}
