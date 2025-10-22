#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct Result {
	int line_count;
	int word_count;
	int symbol_count;
} Result;

Result count_file(FILE* file) {
	Result result = {
		.line_count = 0,
		.word_count = 0,
		.symbol_count = 0,
	};

	char temp;
	int count_words = 0, count_lines = 0, count_symbols = 0;
	while ((temp = getc(file)) != EOF) {
		if (temp == ' ' || temp == '\t' || temp == '\n' || temp == '\v') {
			++count_words;
		}

		if (temp == '\n') {
			++count_lines;
		}

		++count_symbols;
	}

	result.line_count = count_lines;
	result.symbol_count = count_symbols;
	result.word_count = count_words;

	return result;
}

int main(int argc, char** argv) {
	FILE* file;

	if (argc > 1) {
		file = fopen(argv[1], "r");
		if (!file) {
			perror("Couldn`t open file.");
			return 1;
		}
	} else {
		file = stdin;
	}

	Result result = count_file(file);
	printf("%i\n%i\n%i\n", result.line_count, result.word_count, result.symbol_count);
	fclose(file);

	return 0;
}
