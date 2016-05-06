#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"

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
