#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "client.h"

struct Cliet_t {
	char* email;
	int apartment_min_area;
	int apartment_min_rooms;
	int apartment_max_price;
	int total_money_paid;
};

static bool isEmailValid(const char* email);

/**
* Allocates a new client.
*
* Creates a new client. This function receives the client email, apartment
* preferences and retrieves the new client created.
*
* @param email clients email.
* @param apartment_min_area minimal area for the clients wanted apartments
* @param apartment_min_rooms minimal room count in clients wanted apartments
* @param apartment_max_price maximum price for the clients wanted apartments
* @param result pointer to save the result client in
*
* @return
*
* 	CLIENT_OUT_OF_MEMORY - if one email is NULL or does not contain the
* 	character AT_SIGN, apartment_min_area apartment_min_rooms or
* 	apartment_max_price is not bigger then zero, or result is NULL.
* 	CLIENT_OUT_OF_MEMORY - if allocations failed
* 	CLIENT_SUCCESS - in case of success.  A new client is saved in the result
* 	parameter.
*/
ClientResult clientCreate(const char* email, int apartment_min_area,
		int apartment_min_rooms, int apartment_max_price, Client* result) {
	if ((result == NULL) || (!isEmailValid(email)) ||
		(apartment_min_area < 0) || (apartment_min_area < 0) ||
		 (apartment_min_area < 0)) return CLIENT_INVALID_PARAMETERS;
	Client client = malloc (sizeof(Client*));
	if (client == NULL) return CLIENT_OUT_OF_MEMORY;
	client->email = strdup(email);
	if (client->email == NULL) {
		free(client);
		return CLIENT_OUT_OF_MEMORY;
	} else {
		client->total_money_paid = 0;
		*result = client;
		return CLIENT_SUCCESS;
	}
}

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

/**
* ClientDestroy: Deallocates an existing client.
* Clears the element by using the stored free function.
*
* @param client Target client to be deallocated.
* If client is NULL nothing will be done
*/
void clientDestroy(Client client) {
	if (client != NULL) {
		free(client->email);
		free(client);
	}
}

/**
* clientGetMail: gets the given client email.
*
* @param client Target client.
*
* @return
* 	NULL - if client is NULL
* 	The clients email in case of success.
*/
char* clientGetMail(Client client) {
	if (client == NULL) return NULL;
	return client->email;
}

/**
* clientGetMinArea: gets the given client minimal apartment area wanted.
*
* @param client Target client.
*
* @return
* 	NO_CLIENT_VAL - if client is NULL
* 	The clients minimal apartment area wanted in case of success.
*/
int clientGetMinArea(Client client) {
	if (client == NULL) return NO_CLIENT_VAL;
	return client->apartment_min_area;
}

/**
* clientGetMinRooms: gets the given client minimal room count wanted.
*
* @param client Target client.
*
* @return
* 	NO_CLIENT_VAL - if client is NULL
* 	The clients minimal room count wanted in case of success.
*/
int clientGetMinRooms(Client client) {
	if (client == NULL) return NO_CLIENT_VAL;
	return client->apartment_min_rooms;
}

/**
* clientGetMaxPrice: gets the given client maximum price that can be paid for
* an apartment.
*
* @param client Target client.
*
* @return
* 	NO_CLIENT_VAL - if client is NULL
* 	The clients  maximum price that can be paid for an apartment.
*/
int clientGetMaxPrice(Client client) {
	if (client == NULL) return NO_CLIENT_VAL;
	return client->apartment_max_price;
}

/**
* clientAddPaiment: Adds an apartment payment to the given client.
*
* @param client Target client.
* @param payment payment to add.
*
* If client is NULL or payment is negative nothing will be done
*/
void clientAddPayment(Client client, int payment) {
	if ((client != NULL) && (payment > 0)) {
		client->total_money_paid += payment;
	}
}

/**
* clientGetTotalPayments: gets the total payments sum the client payed.
* @param client Target client.
*
* @return
* 	NO_CLIENT_VAL - if client is NULL
* 	or the clients total payments sum in case of success.
*/
int clientGetTotalPayments(Client client) {
	if (client == NULL) return NO_CLIENT_VAL;
	return client->total_money_paid;
}
