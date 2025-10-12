#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct Result {
	int string_count;
	int word_count;
	int symbol_count;
} Result;

FILE* make_buffer_for_stdin(void) {
	FILE* file = fopen("temp.txt", "w+");

	if (!file)
		fprintf(stderr, "Something went wrong.");

	char buffer[255];
	while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		fputs(buffer, file);
	}

	rewind(file);
	return file;
}

int count_strings(FILE* file) {
	int result = 0;

	char temp;
	while ((temp = fgetc(file)) != EOF) {
		if (temp == '\n') ++result;
	}

	rewind(file);
	return result;
}

int count_symbols(FILE* file) {
	int result = 0;

	char temp;
	while ((temp = fgetc(file)) != EOF) ++result;

	rewind(file);
	return result;
}

int count_words(FILE* file) {
	int result = 0;

	char temp;
	while ((temp = fgetc(file)) != EOF) {
		if (temp == ' ' || temp == '\n' || temp == '\t' || temp == '\r' || temp == '\v') ++result;
	}

	rewind(file);
	return result;
}

Result* count_from_file(FILE* file) {
	bool to_remove = false;
	if (file == stdin) {
		file = make_buffer_for_stdin();
		to_remove = true;
	}

	Result* result = (Result*)(malloc(sizeof(Result)));
	if (!result)
		fprintf(stderr, "Something went wrong.");

	if (!file) {
		result->string_count = 0;
		result->symbol_count = 0;
		result->word_count = 0;
	}

	result->symbol_count = count_symbols(file);
	result->string_count = count_strings(file);
	result->word_count = count_words(file);

	if (to_remove) unlink("temp.txt");

	return result;
}

void clean_result(Result* result) {
	free(result);
}

int main(int argc, char** argv) {
	FILE* file;

	if (argc > 1) {
		file = fopen(argv[1], "r");
		if (!file) {
			fprintf(stderr, "Something went wrong.");
		}
	} else {
		file = stdin;
	}

	Result* result = count_from_file(file);
	printf("%i\n%i\n%i\n", result->string_count, result->word_count, result->symbol_count);

	clean_result(result);
	fclose(file);

	return 0;
}
