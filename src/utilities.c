#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"

static int getDigitsCount(int number);

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
bool areStringsEquel(const char *first, const char *second) {
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

