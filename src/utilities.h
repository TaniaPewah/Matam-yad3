#ifndef SRC_UTILITIES_H_
#define SRC_UTILITIES_H_

#define DIGITS_STRING "0123456789"
#define END_OF_STRING '\0'
#define NEGATIVE_SIGN '-'

/*
 * duplicateString: Allocates and duplicates a new copy given string
 *
 * * @param string string to dupicate.
*
* @return
* 	NULL is source string was NULL or allocation failed,
* 	else returns a copy of the string.
 */
char* duplicateString(const char *string);

/*
 * areStringsEqual: checks if two strings are equal using the srtcmp method
 *
 * * @param first first string.
 * * @param second second string.
*
* @return
* 	true if equals; else returns false
 */
bool areStringsEqual(const char *first, const char *second);

/*
* IntToString: gets an intager and returns a string with the number
* this method uses malloc to allocate the string
*
* @param number the number.
*
* @return
* 	NULL if allocation failed; else returns the string
 */
char* IntToString(int number);

/*
* countChar: counts the amount of time a given character appears in a string
*
* @param string the string.
* @param value the character.
*
* @return
* 	0 if string is NULL; else returns the count
 */
int countChar(char* string, char value);

/*
* stringToInt: converts string into int. note that there is no validation
* that the input is correct.
*
* @param string the string.
*
* @return
* 	the number
 */
int stringToInt(const char* string);

#endif /* SRC_UTILITIES_H_ */
