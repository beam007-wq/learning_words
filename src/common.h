#include <panel.h>

#ifndef COMMON
#define COMMON

WINDOW *create_window(const int height, const int width, const int starty, const int startx);
PANEL *create_panel(WINDOW *win);

bool config_read(char **background_color, char **language, int *learn_words_number, int *review_words_number);


typedef struct {
	char *content;

	int row_index;
	int column_index;

	int row_size;
	int column_size;
} Content;

bool config_write(const char *paramater_name, const void *value, const char type);
bool split_dictionary(Content *content, Content *word, Content *meaning);
void random_option(const char *correct_meaning, Content *meaning, char quiz[4][100]);

Content *get_file_content(const char *path);

typedef struct Word_link_list {
    int id;
   	const char *word;
   	const char *meaning;
   	int correct_times;
   	struct Word_link_list *next;
} Word;


Word *load_words(const int learn_word_number, Content *words, Content *meaning);
Word *init_word(const int id, const char *word, const char *meaning, struct Word_link_list *node);
Word *remove_node( struct Word_link_list *n, const int id);
void free_link_list(struct Word_link_list *n);
#endif
