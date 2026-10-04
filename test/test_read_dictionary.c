#include <stdio.h>

void read_dictionary(const char *dictionary_file_path, char content[20][40], int content_index) {
	if (dictionary_file_path == NULL)
		return;

	FILE *read_content = fopen(dictionary_file_path, "r");
	if (read_content == NULL)
		return;
/*
	char content[20][40] = {0};
	int content_index = 0;
*/
	while (fgets(content[content_index], sizeof(content[content_index]), read_content))
		content_index++;
		
	fclose(read_content);
}

int main() {
	char content[20][40] = {0};
	int content_index = 0;

	read_dictionary("../dictionary/dictionary1.txt", content, content_index);

	for (int i = 0; i < 20; i++) 
		printf("%s", content[i]);

	return 0;
}
