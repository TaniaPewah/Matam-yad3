#ifndef SRC_EMAIL_H_
#define SRC_EMAIL_H_

#include <stdbool.h>

#define AT_SIGN '@'

typedef struct Email_t *Email;

/**
* This type defines end codes for the methods.
*/
typedef enum {
	EMAIL_NULL_PARAMETERS = 0,
	EMAIL_INVALID_PARAMETERS = 1,
	EMAIL_OUT_OF_MEMORY = 2,
	EMAIL_SUCCESS = 3
} EmailResult;

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
EmailResult EmailCreate(char* address, Email* result);

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
EmailResult EmailCopy(Email email, Email* result);

/**
* EmailDestroy: Deallocates an existing email.
* Clears the element by using the stored free function.
*
* @param email Target email to be deallocated.
* If email is NULL nothing will be done
*/
void EmailDestroy(Email email);

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
int EmailComapre(Email first, Email second);

/*
 * Checks if both emails are equal.
*
* @param first first email.
* @param second second email.
*
* @return
* 	true if both are NULL or equal, else returns false.
*/
bool EmailAreEqual(Email first, Email second);

#endif /* SRC_EMAIL_H_ */