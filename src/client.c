#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "client.h"

struct Cliet_t {
	Email email;
	int apartment_min_area;
	int apartment_min_rooms;
	int apartment_max_price;
	int total_money_paid;
};

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
*	CLIENT_NULL_PARAMETERS - if email is NULL or result are NULL.
*
* 	CLIENT_INVALID_PARAMETERS - if apartment_min_area, apartment_min_rooms or
* 		apartment_max_price are not bigger then zero.
*
* 	CLIENT_OUT_OF_MEMORY - if allocations failed.
*
* 	CLIENT_SUCCESS - in case of success.
* 		A new client is saved in the result parameter.
*/
ClientResult clientCreate(Email email, int apartment_min_area,
		int apartment_min_rooms, int apartment_max_price, Client* result) {
	if ((result == NULL) || (email == NULL)) return CLIENT_NULL_PARAMETERS;
	if ((apartment_min_area <= 0) || (apartment_min_rooms <= 0)
			|| (apartment_max_price <= 0)) return CLIENT_INVALID_PARAMETERS;
	Client client = malloc (sizeof(*client));
	if (client == NULL) return CLIENT_OUT_OF_MEMORY;
	EmailResult copy_result = emailCopy(email, &(client->email));
	if (copy_result != EMAIL_SUCCESS) {
		free(client);
		return CLIENT_OUT_OF_MEMORY;
	} else {
		client->total_money_paid = 0;
		client->apartment_min_area = apartment_min_area;
		client->apartment_min_rooms = apartment_min_rooms;
		client->apartment_max_price = apartment_max_price;
		*result = client;
		return CLIENT_SUCCESS;
	}
}

/**
* clientCopy: Allocates a new client, identical to the old client
*
* Creates a new client. This function receives a client, and retrieves
* a new identical client pointer in the out pointer parameter.
*
* @param client the original client.
* @param result pointer to save the new client in.
*
* @return
*
* 	CLIENT_NULL_PARAMETERS - if client or pointer are NULL.
*
* 	CLIENT_OUT_OF_MEMORY - if allocations failed.
*
* 	CLIENT_SUCCESS - in case of success. A new client is saved in the result.
*/
ClientResult clientCopy(Client client, Client* result) {
	if ((client == NULL) || (result == NULL)) return CLIENT_NULL_PARAMETERS;
	Client new_client = NULL;
	ClientResult result_state = clientCreate(client->email,
			client->apartment_min_area, client->apartment_min_rooms,
			client->apartment_max_price, &new_client);
	if (result_state != CLIENT_SUCCESS) return  CLIENT_OUT_OF_MEMORY;
	new_client->total_money_paid = client->total_money_paid;
	*result = new_client;
	return CLIENT_SUCCESS;
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
		emailDestroy(client->email);
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
Email clientGetMail(Client client) {
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
