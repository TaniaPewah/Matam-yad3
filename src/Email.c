#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Email.h"

struct Email_t {
	char* address;
};

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
* 	EMAIL_INVALID_PARAMETERS - address contains no AT_SIGN character.
* 	EMAIL_OUT_OF_MEMORY - if allocations failed.
* 	EMAIL_SUCCESS - in case of success.  A new email is saved in the result.
*/
EmailResult EmailCreate(char* address, Email* result) {
	if (address == NULL || result == NULL) return EMAIL_NULL_PARAMETERS;
	if (strchr(address, AT_SIGN) == NULL) return EMAIL_INVALID_PARAMETERS;
	char* adress_copy = strdup(address);
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
* 	EMAIL_INVALID_PARAMETERS - source email address contains no AT_SIGN
* 		character.
* 	EMAIL_OUT_OF_MEMORY - if allocations failed.
* 	EMAIL_SUCCESS - in case of success. A new email is saved in the result.
*/
EmailResult EmailCopy(Email email, Email* result) {
	if (email == NULL) return EMAIL_NULL_PARAMETERS;
	return EmailCreate(email->address, result);
}

/**
* EmailDestroy: Deallocates an existing email.
* Clears the element by using the stored free function.
*
* @param email Target email to be deallocated.
* If email is NULL nothing will be done
*/
void EmailDestroy(Email email) {
	if (email != NULL) {
		free(email->address);
		free(email);
	}
}

/*
 * Compares two Emails by comparing there addresses using srtcmp.
*
* @param first first email.
* @param second second email.
*
* @return
* 	zero if both are equal or both are NULL.
* 	negative value if first is less than second or first is NULL.
* 	positive value if first is less than second or second is NULL.
*/
int EmailComapre(Email first, Email second) {
	if ((first == NULL) && (second == NULL)) return 0;
	if (first == NULL)  return -1;
	if (second == NULL) return  1;
	return strcmp(first->address, second->address);
}

/*
 * Checks if both emails are equal.
*
* @param first first email.
* @param second second email.
*
* @return
* 	true if both are NULL or equal, else returns false.
*/
bool EmailAreEqual(Email first, Email second) {
	return (EmailComapre(first, second) == 0);
}
