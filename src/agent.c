/*
 * agent.c
 *
 *  Created on: Apr 30, 2016
 *      Author: Tania
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "agent.h";
#include "map.h";

struct Agent_t{
	char* email;
	char* companyName;
	int taxPercentge;
	Map* apartmentServices;
};


static bool isEmailValid(const char* email);


/**
* isMainValied: checks if the given email adress is Valid.
*
* @param email email to check.
*
* @return
* 	false if email is NULL or does not contian AT_SIGN, else return true
*/
static bool isEmailValid(const char* email) {
	return ((email != NULL) && (strchr(email, AT_SIGN) != NULL));
}
