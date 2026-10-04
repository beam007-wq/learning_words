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
#endif
