#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Word_link_list {
	int id;
	const char *word;
	const char *meaning;
	struct Word_link_list *next;
};

struct Word_link_list *create(int id, const char *word, const char *meaning, struct Word_link_list *node) {
	if (word == NULL || meaning == NULL) 
		return NULL;

	struct Word_link_list *a = (struct Word_link_list *)malloc(sizeof(struct Word_link_list));
	if (a == NULL)
		return NULL;

	a->id = id;
	a->word = word;
	a->meaning = meaning;
	a->next = node;

	return a;
}

void print_node(struct Word_link_list *instance) {
	struct Word_link_list *temp = instance;

	while (temp) {
		if (temp != NULL)
			printf("%d:%s ", temp->id, temp->word);

		temp = temp->next;
	}	

	printf("\n");
}

void print_loop_link_list(struct Word_link_list *instance) {
	struct Word_link_list *temp = instance;
	struct Word_link_list *tail = instance;

	while (tail->next != temp) {
		printf("%d:%s ", tail->id, tail->word);
		tail = tail->next;		
	}
	printf("%d:%s", tail->id, tail->word);

	printf("\n");
}

void free_all_node(struct Word_link_list *instance) {
	struct Word_link_list *temp = instance;
	struct Word_link_list *tail = instance;

	while (tail->next != temp)
		tail = tail->next;

	tail->next = NULL;	

	while (temp) {
		struct Word_link_list *next = temp->next;
		free(temp);
		temp = next;
	}
}

void free_all_node2(struct Word_link_list *instance) {
	struct Word_link_list *temp = instance;

	while (temp) {
		struct Word_link_list *next = temp->next;
		free(temp);
		temp = next;
	}
}

void remove_node(struct Word_link_list *instance, const int id) {
	struct Word_link_list *temp = instance; 

	while (temp) {
		struct Word_link_list *next = temp->next;

		if (temp->id == id) {
			next = next->next;	
			free(temp);
			break;
		}
		
		temp = next;	
	}
}

void remove_node2(struct Word_link_list *instance, const int id) {
	struct Word_link_list *temp = instance;

	while (temp) {
		struct Word_link_list *next = temp->next;
		if (temp->id == id) {
			free(temp);
			temp = next->next;
			break;
		}

		temp = next;
	}
}

int main() {
	const char *words[4] = {"hello", "world", "hi", "test"};		
	const char *meaning[4] = {"你好", "世界", "嗨", "测试"};

	struct Word_link_list *instance = NULL;

	for (int i = 0; i < 4; i++)
		instance = create(i, words[i], meaning[i], instance);

	print_node(instance);

	/*
	struct Word_link_list *tail = instance;
	while (tail->next != NULL) 
		tail = tail->next;

	tail->next = instance;
	instance = tail;

	print_loop_link_list(instance);
	remove_node(instance, 1);
	print_loop_link_list(instance);

	free_all_node(instance);

	*/

	remove_node2(instance, 1);
	print_node(instance);

	free_all_node2(instance);

	return 0;
}
