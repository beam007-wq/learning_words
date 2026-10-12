#include <panel.h>

#ifndef COMMON
#define COMMON

WINDOW *create_window(const int height, const int width, const int starty, const int startx);
PANEL *create_panel(WINDOW *win);

bool config_read(char *background_color,
                 int background_color_length,
                 char *language,
                 int language_length,
                 int *learn_words_number,
                 int *review_words_number);

bool config_write(const char *paramater_name, const void *value, const char type);
bool split_dictionary(char content[20][100], char word[20][100], int word_index, char meaning[20][100], int meaning_index);
int random_number(int **last_random_number);
void random_option(const char *correct, const char meaning[20][100], char quiz[4][100]);

typedef struct Word_link_list {
    int id;
   	const char *word;
   	const char *meaning;
   	int correct_times;
   	struct Word_link_list *next;
} Word;

Word *init_word(const int id, const char *word, const char *meaning, struct Word_link_list *node);
Word *remove_node( struct Word_link_list *n, const int id);
void free_link_list(struct Word_link_list *n);
#endif
