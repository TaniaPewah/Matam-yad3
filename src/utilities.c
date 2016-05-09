#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "utilities.h"



static int getDigitsCount(int number);
//static char* getSubString(char* str, int start_index, int end_index);

/*
 * duplicateString: Allocates and duplicates a new copy given string
 *
 * * @param string string to dupicate.
*
* @return
* 	NULL is source string was NULL or allocation failed,
* 	else returns a copy of the string.
 */
char* duplicateString(const char *string)
{
	if (string == NULL) return NULL;
	char *result = malloc((strlen(string) * sizeof(char)) + 1);
	if (result != NULL) strcpy(result, string);
	return result;
}

/*
 * areStringsEquel: checks if two strings are equal using the srtcmp method
 *
 * * @param first first string.
 * * @param second second string.
*
* @return
* 	true if equals; else returns false
 */
bool areStringsEqual(const char *first, const char *second) {
	return (strcmp(first, second) == 0);
}

/*
* IntToString: gets an intager and returns a string with the number
* this method uses malloc to allocate the string
*
* @param number the number.
*
* @return
* 	NULL if allocation failed; else returns the string
 */
char* IntToString(int number) {
	int length = getDigitsCount(number);
	int word_length = length + (number < 0 ? 2 : 1);
	char* result = malloc(sizeof(char) * (word_length));
	if (result == NULL) return NULL;
	int start_index = 0;
	if (number < 0) {
		result[0] = NEGATIVE_SIGN;
		start_index++;
	}
	result[word_length - 1] = END_OF_STRING;
	for (int i = word_length - 2; i >= start_index; i--) {
		result[i] = DIGITS_STRING[number % 10];
		number /= 10;
	}
	return result;
}

/*
* getDigitsCount: gets an intager and returns its digits count
*
* @param number the number.
*
* @return
* the digits count
 */
static int getDigitsCount(int number) {
	int count = 0;
	do {
		count++;
		number /= 10;
	} while (number != 0);
	return count;
}

/*
* countChar: counts the amount of time a given character appears in a string
*
* @param string the string.
* @param value the character.
*
* @return
* 	0 if string is NULL; else returns the count
 */
int countChar(char* string, char value) {
	if (string == NULL) return 0;
	int count = 0, curr_index = 0;
	while (string[curr_index] != '\0') {
		count += string[curr_index] == value ? 1 : 0;
		curr_index++;
	}
	return count;
}


char** commandSplit(char* string_to_split, int *size, const char separator) {
   /* char** result = NULL;
    size = countChar(string_to_split, separator) + 2;
    int start_index = 0, last_index = 0, current_index = 0, item_index = 0;
    result = malloc(sizeof(char*) * size);
    if (result == NULL) return NULL;
    for (int i = 0; i < size; i++) result[i] = NULL;
    while (string_to_split[current_index] != END_OF_LINE) {
    	if (string_to_split[current_index] != separator) {
    		last_index++;
    	} else {
    		if (start_index < last_index) {
				result[item_index] = getSubString(string_to_split, start_index,
						last_index);
			}
    		start_index = current_index + 1;
    		last_index = current_index + 1;
    	}
    	current_index++;
    }
    if (start_index < last_index) {
		result[item_index] = getSubString(string_to_split, start_index,
				last_index);
	}*/

    return NULL;
}

/*static char* getSubString(char* str, int start_index, int end_index) {
	char* new_string = malloc(sizeof(char) * ((end_index - start_index) + 2));
	if (new_string == NULL) return NULL;
	for (int i = 0; i < (end_index - start_index + 1); i++) {
		new_string[i] = str[start_index + i];
	}
	new_string[end_index - start_index + 1] = END_OF_LINE;
	return new_string;
}*/

void matrixDestroy(char** matrix, int size) {
	for (int i = 0; i < size; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

