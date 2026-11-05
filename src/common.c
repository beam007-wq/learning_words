//common.c

#include <panel.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "common.h"

#define LANGUAGE_NUM 2
#define BACKGROUND_COLOR_KEYWORDS_NUM 6
#define LEARN_MAX 20
#define REVIEW_MAX 100

const char *config_filepath = "../config";

struct Config_AST {
	char *name;
	int name_index;
	int name_size;

	char sign;
	struct {
		char type;

		int paramater_index;
		int paramater_size;

		union {
			int number;
			char *paramater;
		} value;

	} get_value;

	char end;
};

struct Split {
	char sign;
	char end;
};

enum Config_name {
	BACKGROUND_COLOR = 0,
	LANAGUAGE = 1,
	LEARN_WORDS_NUMBER = 2,
	REVIEW_WORDS_NUMBER = 3,
};

const char *valid_keywords[4] = {
	"background_color",
	"language",
	"learn_words_number",
	"review_words_number"
};

WINDOW *create_window(const int height, const int width, const int starty, const int startx) {
        if (height == 0 || width == 0 || startx < 0 || starty < 0) return NULL;

        WINDOW *win = newwin(height,width,starty,startx);
        if (win == NULL) return NULL;

        return win;
}

PANEL *create_panel(WINDOW *win) {
	if (win == NULL) return NULL;

	PANEL *panel = new_panel(win);
	if (panel == NULL) return NULL;

	return panel;
}

static bool check_paramater_name(const char *paramater_name) {
	for (int i = 0; i < 4; i++)
		if (strcmp(paramater_name, valid_keywords[i]) == 0)
			return true;

	return false;
}

static bool check_paramater_value(const char *paramater_name, const void *value, const char type) {
	const char *background_color[6] = {"red", "black", "blue", "green", "yellow"};
	const char *language[2] = {"zh_CN", "en_US"};

	int number = 0;
	char *paramater_value = NULL;

	if (type == 's') {
		const int length = strlen((const char *)value);
		paramater_value = (char *)malloc(length + 1);
		snprintf(paramater_value, length + 1, "%s", (char *)value);
	} else if (type == 'n') {
		number = *(int *)value;
	} else {
		return false;
	}

	if (type == 's') {
		if (strcmp(paramater_name, valid_keywords[BACKGROUND_COLOR]) == 0) {
			for (int i = 0; i < BACKGROUND_COLOR_KEYWORDS_NUM; i++) {
				if (strcmp(paramater_value, background_color[i]) == 0) {
					free(paramater_value);
					return true;
				}
			}
		} else if (strcmp(paramater_name, valid_keywords[LANAGUAGE]) == 0) {
			for (int i = 0; i < LANGUAGE_NUM; i++) {
				if (strcmp(paramater_value, language[i]) == 0) {
					free(paramater_value);
					return true;
				}
			}
		}
	} else {
		if (strcmp(paramater_name, valid_keywords[LEARN_WORDS_NUMBER]) == 0) {
			if (number > 0 && number < LEARN_MAX)
				return true;
		} else if (strcmp(paramater_name, valid_keywords[REVIEW_WORDS_NUMBER]) == 0) {
			if (number > 0 && number < REVIEW_MAX)
				return true;
		}
	}

	return false;
}

Content *get_file_content(const char *path) {
	Content *get_content = (Content *)malloc(sizeof(Content));
	if (get_content == NULL)
		return NULL;
		
	get_content->column_index = 0;
	get_content->row_index = 0;
	get_content->column_size = 10;
	get_content->row_size = 1;
	get_content->content = (char *)malloc(get_content->row_size * get_content->column_size + 1);

	FILE *read_content = fopen(path, "r");
	if (read_content == NULL)
		return NULL;

	char c = '\0';
	while ((c = fgetc(read_content)) != EOF) {
		if (c == '\n') {
			get_content->content[get_content->row_index * get_content->column_size + get_content->column_index] = '\0';
			get_content->column_index = 0;
			get_content->row_index += 1;
			continue;
		}

		if (get_content->column_index >= get_content->column_size) {
			get_content->column_size *= 10;
			char *temp = (char *)realloc(get_content->content, get_content->row_size * get_content->column_size);
			if (temp == NULL)
				return false;
			get_content->content = temp;
		}

		if (get_content->row_index >= get_content->row_size) {
			get_content->row_size += 1;
			char *temp = (char *)realloc(get_content->content, get_content->row_size * get_content->column_size);
			if (temp == NULL)
				return false;
			get_content->content = temp;
		}

		get_content->content[get_content->row_index * get_content->column_size + get_content->column_index] = c;
		get_content->column_index += 1;	
	}
		
	fclose(read_content);

	return get_content;
}

bool config_read(char **background_color, char **language, int *learn_words_number, int *review_words_number) {
	bool result = false;

	Content *get_content = get_file_content(config_filepath);
	if (get_content == NULL)
		return false;

	//init AST
	struct Config_AST ast = {
		.name_size = 2,
	    .name_index = 0,
	    .sign = '=',
	    .end = ';',
	    .get_value.paramater_index = 0,
		.get_value.paramater_size = 5
	};

	ast.name = (char *)malloc(ast.name_size + 1);
	if (ast.name == NULL)
		return false;

	
	for (int i = 0; i < get_content->row_index; i++)	{
		ast.get_value.paramater_index = 0;
		ast.name_index = 0;
		ast.name[0] = '\0';

		ast.get_value.value.paramater = (char *)malloc(ast.get_value.paramater_size + 1);
		if (ast.get_value.value.paramater == NULL)
			return false;

		char get_number[2+1] = {'\0'};

		int get_content_length = strlen(&get_content->content[i * get_content->column_size]);
		int get_number_index = 0;
		int quote_sign = 0;

		bool equal_sign = false, is_number = false, is_quote = false;

		for (int e = 0; e < get_content_length; e++) {
			if (ast.name_index >= ast.name_size) {
				ast.name_size *= 1;
				char *temp = (char *)realloc(ast.name, ast.name_size + 1);
				if (temp == NULL)
					return false;
				
				ast.name = temp;
			}

			//check equal_sign
			if (get_content->content[i * get_content->column_size + e] == ast.sign) {
				equal_sign = true;
				ast.name[ast.name_index] = '\0';
				continue;
			}

			if (get_content->content[i * get_content->column_size + e] == '/')
				break;

			//check and write paramater name
			if (isspace(get_content->content[i * get_content->column_size + e]) == 0 && equal_sign == false) {
				ast.name[ast.name_index] = get_content->content[i * get_content->column_size + e];
				ast.name_index++;
			} else if (isspace(get_content->content[i * get_content->column_size + e]) == 0 && equal_sign == true) {
				//The loop that encounter semicolon to exit
				if (get_content->content[i * get_content->column_size + e] == ast.end) {
					ast.get_value.value.paramater[ast.get_value.paramater_index] = '\0';
					get_number[get_number_index] = '\0';
					break;
				}

				//check type
				if (isdigit(get_content->content[i * get_content->column_size + e])) {
					ast.get_value.type = 'n';
					is_number = true;
				} else if (get_content->content[i * get_content->column_size + e] == '"') {
					is_quote = true;
					ast.get_value.type = 's';
				}

				//String check and write
				if (get_content->content[i * get_content->column_size + e] == '"' && is_quote == true) {
					quote_sign++;
					continue;
				} else if (quote_sign == 1 && is_quote== true) {
					if (ast.get_value.paramater_index >= ast.get_value.paramater_size) {
						ast.get_value.paramater_size *= 2;
						char *temp = (char *)realloc(ast.get_value.value.paramater, ast.get_value.paramater_size + 1);
						if (temp == NULL)
							return false;
						ast.get_value.value.paramater = temp;
					}

					ast.get_value.value.paramater[ast.get_value.paramater_index] = get_content->content[i * get_content->column_size + e];
					ast.get_value.paramater_index++;
				}

				//number check and write
				if (isdigit(get_content->content[i * get_content->column_size + e]) && is_number == true && get_number_index < 2) {
					get_number[get_number_index] = get_content->content[i * get_content->column_size + e];
					get_number_index++;
				}
			} else {
				continue;
			}
		}
		
		if (get_number[0] != '\0')
			ast.get_value.value.number = (int)strtol(get_number, NULL, 10);

		if (ast.name[0] != '\0' && check_paramater_name(ast.name)) {
			if (is_number) {
				if (!check_paramater_value(ast.name, &ast.get_value.value.number, 'n'))
					return false;
			} else if (is_quote) {
				if (!check_paramater_value(ast.name, ast.get_value.value.paramater, 's'))
					return false;
			} else {
				return false;
			}
			
			if (ast.get_value.type == 's') {
				const int length = strlen(ast.get_value.value.paramater);
				if (strcmp(ast.name, valid_keywords[BACKGROUND_COLOR]) == 0) {	
					*background_color = (char *)malloc(length + 1);
					snprintf(*background_color, length + 1, "%s", ast.get_value.value.paramater);
					result = true;
				} else if (strcmp(ast.name, valid_keywords[LANAGUAGE]) == 0) {
					*language = (char *)malloc(length + 1);
					snprintf(*language, length + 1, "%s", ast.get_value.value.paramater);
					result = true;
				} else {
					free(ast.get_value.value.paramater);
					return false;
				}

				free(ast.get_value.value.paramater);
			} else {
				if (strcmp(ast.name, valid_keywords[LEARN_WORDS_NUMBER]) == 0) {
					*learn_words_number = ast.get_value.value.number;
					result = true;
				} else if (strcmp(ast.name, valid_keywords[REVIEW_WORDS_NUMBER]) == 0) {
					*review_words_number = ast.get_value.value.number;
					result = true;
				} else {
					return false;
				}
			}
		}

		ast.get_value.value.number = 0;
	}		

	free(ast.name);
	free(get_content->content);
	free(get_content);

	return result;
}

bool config_write(const char *paramater_name, const void *value, const char type) {
	if (!check_paramater_name(paramater_name) || !check_paramater_value(paramater_name, value, type))
		return false;

	Content *get_content = get_file_content(config_filepath);
	if (get_content == NULL)
		return false;

	//select which wants to modify
	int target = -1;
 	for (int i = 0; i < get_content->row_index; i++) {
		char *keyword = strstr(&get_content->content[i * get_content->column_size], paramater_name);

		if (keyword != NULL) {
			target = i;
			break;
		}
	}

	//if we not found
	if (target == -1)
		return false;	

	int check = get_content->column_size - (int)strlen(&get_content->content[target * get_content->column_size]);
	if (type == 's') {
		char *modify_str = (char *)value;

		char *modify = strstr(&get_content->content[target * get_content->column_size], "\"");
		if (modify == NULL) 
			return false;

		//eliminate
		*modify = '\0';

		int value_length = (int)strlen(modify_str) + 4; // 1(") + 1(") + 1(;) + 1(\0)
		if (value_length >= check) {
			get_content->column_size *= 2;
			char *temp = (char *)realloc(get_content->content, get_content->row_size * get_content->column_size);
			if (temp == NULL)
				return false;
			get_content->content = temp;
		}

		snprintf(modify, value_length, "\"%s\";", modify_str);	
	} else {
		int modify_number= *(int *)value;

		char *modify = strstr(&get_content->content[target * get_content->column_size], "=");
		if (modify == NULL)
			return false;
		
		modify++; //pass = symbol
		*modify = '\0'; //and then eliminate

		int value_length = 5;
		if (value_length >= check) {
			get_content->column_size *= 2;
			char *temp = (char *)realloc(get_content->content, get_content->row_size * get_content->column_size);
			if (temp == NULL)
				return false;
			get_content->content = temp;
		}

		snprintf(modify, value_length, " %d;", modify_number);
	}

	FILE *write = fopen(config_filepath, "w");
	if (write == NULL)
		return false;

	for (int i = 0; i < get_content->row_size; i++) {
		if (fputs(&get_content->content[i * get_content->column_size], write) == EOF)
			break;

		fputc('\n', write);
	}

	fclose(write);

	free(get_content->content);
	free(get_content);

	return true;
}

bool split_dictionary(Content *content, Content *word, Content *meaning) {
	if (content == NULL || word == NULL || meaning == NULL)
		return false;

	struct Split split_content = {
		.sign = ',',
		.end = ';'
	};

	for (int i = 0; i < content->row_size; i++) {
		bool sign = false;

		for (int e = 0; e < content->column_size; e++) {
			if (!sign) {
				if (word->column_index >= word->column_size) {
					word->column_size *= 2;
					char *temp = (char *)realloc(word->content, word->row_size * word->column_size);
					if (temp == NULL)
						return false;
					word->content = temp;
				}

				if (word->row_index >= word->row_size) {
					word->row_size += 1;
					char *temp = (char *)realloc(word->content, word->row_size * word->column_size);
					if (temp == NULL)
						return false;
					word->content = temp;
				}

				if (content->content[i * content->column_size + e] == split_content.sign) {
					sign = true;					
					word->content[word->row_index * word->column_size + word->column_index] = '\0';
					word->column_index = 0;
					word->row_index += 1;
					continue;
				}

				word->content[word->row_index * word->column_size + word->column_index] = content->content[i * content->column_size + e];
				word->column_index += 1;
			} else {
				if (meaning->column_index >= meaning->column_size) {
					meaning->column_size *= 2;
					char *temp = (char *)realloc(meaning->content, meaning->row_size * meaning->column_size);
					if (temp == NULL)
						return false;
					meaning->content = temp;
				}

				if (meaning->row_index >= meaning->row_size) {
					meaning->row_size += 1;
					char *temp = (char *)realloc(meaning->content, meaning->row_size * meaning->column_size);
					if (temp == NULL)
						return false;
					meaning->content = temp;
				}

				if (content->content[i * content->column_size + e] == split_content.end) {
					meaning->content[meaning->row_index * meaning->column_size + meaning->column_index] = '\0';
					meaning->column_index = 0;
					meaning->row_index += 1;
					break;
				}

				meaning->content[meaning->row_index * meaning->column_size + meaning->column_index] = content->content[i * content->column_size + e];
				meaning->column_index += 1;
			}
		}
	}

	return true;
}

void random_option(const char *correct_meaning, Content *meaning, char quiz[4][100]) {
	int correct_random_num = rand() % 4;
	snprintf(quiz[correct_random_num], 100, "%s", correct_meaning);

	int last_random_num = -1;
	int i = 0;
	while (i <= 3) {
		int random_num = rand() % 10;

		if (i == correct_random_num) {
			i++;
			continue;
		}

		if (random_num != last_random_num && strcmp(correct_meaning, &meaning->content[random_num * meaning->column_size]) != 0) {
			last_random_num = random_num;
			snprintf(quiz[i], 100, "%s", &meaning->content[random_num * meaning->column_size]);
			i++;
		}
	}
}

Word *load_words(const int learn_word_number, Content *words, Content *meaning) {
    Word *word = NULL;

    for (int i = 0; i < learn_word_number; i++) {
        word = init_word(i, &words->content[i * words->column_size], &meaning->content[i * meaning->column_size], word);
        if (word == NULL)
            return NULL;
    }

    return word;
}

struct Word_link_list *init_word(const int id, const char *word, const char *meaning, struct Word_link_list *node) {
	if (word == NULL || meaning == NULL)
		return NULL;

	struct Word_link_list *new_node = (struct Word_link_list *)malloc(sizeof(struct Word_link_list));
	if (new_node == NULL)
		return NULL;

	new_node->id = id;
	new_node->word = word;
	new_node->meaning = meaning;
	new_node->correct_times = 1;
	new_node->next = NULL;

	if (node == NULL)
		return new_node;

	struct Word_link_list *head = node;

	if (node->next == NULL) {
		node->next = new_node;
		new_node->next = head;
		return node;
	}

	struct Word_link_list *temp = node;

	while (temp->next != head)
		temp = temp->next;

	temp->next = new_node;
	new_node->next = head;

	return node;
}

struct Word_link_list *remove_node(struct Word_link_list *n, const int id) {
	struct Word_link_list *temp = n;
	struct Word_link_list *head = n;
	
	if (n->next == head && n->next->id == id) {
		free(temp);
		return NULL;
	}
	
	if (id == head->id)	{
		struct Word_link_list *tail = n;
		while (tail->next != head)
			tail = tail->next;
			
		struct Word_link_list *next_head = head->next;
		tail->next = next_head;	
		free(head);

		return next_head;
	}

	struct Word_link_list *prev = temp;

	while (temp->next != head) {
		struct Word_link_list *next = temp->next;
		if (temp->id == id) {
			prev->next = next;
			free(temp);
			return n;
		}

		prev = temp;
		temp = next;
	}

	return n;
}

void free_link_list(struct Word_link_list *n) {
	struct Word_link_list *head = n;
	struct Word_link_list *tail = n;

	while (tail->next != head)
		tail = tail->next;
	tail->next = NULL;

	struct Word_link_list *temp = n;

	while (temp) {
		struct Word_link_list *next = temp->next;
		free(temp);
		temp = next;
	}
}
