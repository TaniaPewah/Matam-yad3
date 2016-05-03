#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#define NO_CLIENT_VAL -1
#define AT_SIGN '@'

#include "Email.h"

/**
* This type defines end codes for the methods.
*/
typedef enum {
	CLIENT_OUT_OF_MEMORY = 0,
	CLIENT_NULL_PARAMETERS = 1,
	CLIENT_INVALID_PARAMETERS = 2,
	CLIENT_SUCCESS = 3
} ClientResult;

typedef struct Cliet_t *Client;

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
		int apartment_min_rooms, int apartment_max_price, Client* result);


/**
* ClientDestroy: Deallocates an existing client.
* Clears the element by using the stored free function.
*
* @param client Target client to be deallocated.
* If client is NULL nothing will be done
*/
void clientDestroy(Client client);

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
* 	EMAIL_NULL_PARAMETERS - if client or pointer are NULL.
*
* 	EMAIL_OUT_OF_MEMORY - if allocations failed.
*
* 	EMAIL_SUCCESS - in case of success. A new client is saved in the result.
*/
ClientResult clientCopy(Client client, Client* result);

/**
* clientGetMail: gets the given client email.
*
* @param client Target client.
*
* @return
* 	NULL - if client is NULL
* 	The clients email in case of success.
*/
Email clientGetMail(Client client);

/**
* clientGetMinArea: gets the given client minimal apartment area wanted.
*
* @param client Target client.
*
* @return
* 	NO_CLIENT_VAL - if client is NULL
* 	The clients minimal apartment area wanted in case of success.
*/
int clientGetMinArea(Client client);

/**
* clientGetMinRooms: gets the given client minimal room count wanted.
*
* @param client Target client.
*
* @return
* 	NO_CLIENT_VAL - if client is NULL
* 	The clients minimal room count wanted in case of success.
*/
int clientGetMinRooms(Client client);

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
int clientGetMaxPrice(Client client);

/**
* clientAddPaiment: Adds an apartment payment to the given client.
*
* @param client Target client.
* @param payment payment to add.
*
* If client is NULL or payment is negative nothing will be done
*/
void clientAddPayment(Client client, int payment);

/**
* clientGetTotalPayments: gets the total payments sum the client payed.
* @param client Target client.
*
* @return
* 	NO_CLIENT_VAL - if client is NULL
* 	or the clients total payments sum in case of success.
*/
int clientGetTotalPayments(Client client);

#endif /* SRC_CLIENT_H_ */
