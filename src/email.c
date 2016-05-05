#include "email.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct Email_t {
	char* address;
};

static char* duplicateString(const char *str);

/**
* Allocates a new email.
*
* Creates a new email. This function receives the email address, retrieves the
* new email element pointer in the out pointer parameter.
*
* @param address the email address.
* @param result pointer to save the result email in
*
* @return
*
* 	EMAIL_NULL_PARAMETERS - if email address or pointer are NULL.
*
* 	EMAIL_INVALID_PARAMETERS - address contains no AT_SIGN character.
*
* 	EMAIL_OUT_OF_MEMORY - if allocations failed.
*
* 	EMAIL_SUCCESS - in case of success.  A new email is saved in the result.
*/
EmailResult emailCreate(char* address, Email* result) {
	if (address == NULL || result == NULL) return EMAIL_NULL_PARAMETERS;
	if (strchr(address, AT_SIGN) == NULL) return EMAIL_INVALID_PARAMETERS;
	char* adress_copy = duplicateString(address);
	if (adress_copy == NULL) return EMAIL_OUT_OF_MEMORY;
	Email mail = malloc(sizeof(*mail));
	if (mail == NULL) {
		free(adress_copy);
		return EMAIL_OUT_OF_MEMORY;
	} else {
		mail->address = adress_copy;
		*result = mail;
		return EMAIL_SUCCESS;
	}
}

/**
* Allocates a new email, identical to the old email
*
* Creates a new email. This function receives an email elemnet, and retrieves
* a new identical email element pointer in the out pointer parameter.
*
* @param email the original email.
* @param result pointer to save the new email in.
*
* @return
*
* 	EMAIL_NULL_PARAMETERS - if email or pointer are NULL.

* 	EMAIL_OUT_OF_MEMORY - if allocations failed.
*
* 	EMAIL_SUCCESS - in case of success. A new email is saved in the result.
*/
EmailResult emailCopy(Email email, Email* result) {
	if ((email == NULL) || (result == NULL)) return EMAIL_NULL_PARAMETERS;
	return emailCreate(email->address, result);
}

/**
* EmailDestroy: Deallocates an existing email.
* Clears the element by using the stored free function.
*
* @param email Target email to be deallocated.
*
* If email is NULL nothing will be done
*/
void emailDestroy(Email email) {
	if (email != NULL) {
		free(email->address);
		free(email);
	}
}

/*
 * EmailComapre: Compares two Emails by comparing there addresses using srtcmp.
*
* @param first first email.
* @param second second email.
*
* @return
*
* 	zero if both are equal or both are NULL.
*
* 	negative value if first is less than second or first is NULL.
*
* 	positive value if first is less than second or second is NULL.
*/
int emailComapre(Email first, Email second) {
	if ((first == NULL) && (second == NULL)) return 0;
	if (first == NULL)  return -1;
	if (second == NULL) return  1;
	return strcmp(first->address, second->address);
}

/*
 * EmailAreEqual: Checks if both emails are equal.
*
* @param first first email.
* @param second second email.
*
* @return
* 	true if both are NULL or equal, else returns false.
*/
bool emailAreEqual(Email first, Email second) {
	return (emailComapre(first, second) == 0);
}

/*
 * emailToString: returns a copy of the email address.
 * this method uses malloc in order to allocate memory to save new string in.
*
* @param email the email.
*
* @return
* 	NULL if email is NULL or memory allocation failed. else the email address.
*/
char* emailToString(Email email) {
	if (email == NULL) return NULL;
	return duplicateString(email->address);
}

/*
 * duplicateString: Allocates and duplicates a new copy given string
 *
 * * @param string string to dupicate.
*
* @return
* 	NULL is source string was NULL or allocation failed,
* 	else returns a copy of the string.
 */
static char* duplicateString(const char *string)
{
	if (string == NULL) return NULL;
	char *result = malloc((strlen(string) * sizeof(char)) + 1);
	if (result != NULL) strcpy(result, string);
	return result;
}
