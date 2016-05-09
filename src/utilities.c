#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "utilities.h"

static int getDigitsCount(int number);
char** str_split(char* string_to_split, const char separator);

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

char** str_split(char* string_to_split, const char separator){
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = string_to_split;
    char* last_separator = 0;
    char delim[2];
    delim[0] = separator;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (separator == *tmp){
            count++;
            last_separator = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_separator < (string_to_split + strlen(string_to_split) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;
    result = malloc(sizeof(char*) * count);

    if (result){
        size_t idx  = 0;
        char* token = strtok(string_to_split, delim);

        while (token){
            if(!(idx < count)){
            	free(result);
            	return NULL;
            }

            *(result + idx++) = duplicateString(token);
            token = strtok(0, delim);
        }
        if(!(idx == count - 1)){
        	free(result);
        	return NULL;
        }
        *(result + idx) = 0;
    }

    return result;
}
/*
int main()
{
    char months[] = "JAN,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC";
    char** tokens;

    printf("months=[%s]\n\n", months);

    tokens = str_split(months, ',');

    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("month=[%s]\n", *(tokens + i));
            free(*(tokens + i));
        }
        printf("\n");
        free(tokens);
    }

    return 0;
}*/
