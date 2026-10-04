#include "../src/common.h"
#include <stdio.h>

int main() {
	char background_color[20+1];
	int background_color_length = 20;
	char language[20+1];
	int language_length = 20;
	int learn_words_number = 0;
	int review_words_number = 0;

	if (!config_read(background_color, background_color_length, language, language_length, &learn_words_number, &review_words_number))
		return 1;
	return 0;
}
