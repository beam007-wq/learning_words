//learn.c

#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"
#include "learn.h"

static bool read_dictionary(const char *dictionary_file_path, char content[20][100], int content_index) {
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

static void draw_display_learn_number(WINDOW *win, int number) {
	if (win == NULL) return;

	box(win,0,0);
	waddstr(win,"remains");
	mvwprintw(win,1,0, "%d", number);
	wrefresh(win);
}

static void draw_learn_zone(WINDOW *win, Word *w, char quiz[4][100], char words[20][100], char meaning[20][100], int *last_random_num) {
	if (win == NULL && w == NULL) return;

	box(win,0,0);
	mvwaddstr(win,0,0,"learn zone");
	mvwprintw(win,2,10,"%d", w->correct_times - 1);

	int random_word = random_number(&last_random_num);
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

Word *load_words(const int learn_word_number, const char words[20][100], const char meaning[20][100], int *last_random_num) {
    Word *word = NULL;

    for (int i = 0; i < learn_word_number; i++) {
        int random_word = random_number(&last_random_num);

        word = init_word(i, words[random_word], meaning[random_word], word);
        if (word == NULL)
            return NULL;
    }

    return word;
}

void access_learn(const int learn_word_number, const char *dictionary_file_path) {
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


	if (learn_title != NULL && display_number != NULL && learn_zone != NULL) {
		draw_learn_title(learn_title);

		char quiz[4][100] = {0};
		int select = 0;
		int last_random_num = -1;
		int word_number = learn_word_number;

		Word *word = load_words(learn_word_number, words, meaning, &last_random_num);
		if (word == NULL)
		    return;

		while (1) {
			werase(learn_zone);
			werase(display_number);

			if (word == NULL)
			    break;

			draw_display_learn_number(display_number, word_number);
			draw_learn_zone(learn_zone, word, quiz, words, meaning, &last_random_num);

			if (word->correct_times > 2) {
			    word = remove_node(word, word->id);
				word_number--;
			}

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

	delwin(learn_title);
	delwin(display_number);
	delwin(learn_zone);
}
