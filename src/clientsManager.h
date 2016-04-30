#ifndef SRC_CLIENTSMANAGER_H_
#define SRC_CLIENTSMANAGER_H_

#include "client.h"

/**
* This type defines end codes for the methods.
*/
typedef enum {
	CLIENT_MANAGRE_OUT_OF_MEMORY = 0,
	CLIENT_MANAGRE_INVALID_PARAMETERS = 1,
	CLIENT_MANAGRE_ALREADY_EXISTS = 2,
	CLIENT_MANAGRE_NOT_EXISTS = 3,
	CLIENT_MANAGRE_SUCCESS = 4
} ClientsManagerCode;

typedef struct clientsManager_t *ClientsManager;

/**
* Allocates a new ClientsManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new clients in case of success.
*/
ClientsManager clientsManagerCreate();

/**
* clientsManagerDestroy: Deallocates an existing manager.
* Clears the element by using the stored free function.
*
* @param manager Target clients Manager to be deallocated.
* If manager is NULL nothing will be done
*/
void clientsManagerDestroy(ClientsManager manager);

/**
* clientsManagerAddClient: adds the new client to the collection.
*
* @param manager Target clients Manager to add to.
* @param client Target client to add.
*
* @return
* 	CLIENT_MANAGRE_INVALID_PARAMETERS - if client is NULL.
* 	CLIENT_MANAGRE_ALREADY_EXISTS - if client email already registered.
* 	CLIENT_MANAGRE_SUCCESS - in case of success.
*/
ClientsManagerCode clientsManagerAdd(ClientsManager manager, Client client);

/**
* clientsManagerRemove: removes the given client from the collection.
* note that the client will not be deallocated!
*
* @param manager Target clients Manager to remove from.
* @param client Target client to remove.
*
* @return
* 	CLIENT_MANAGRE_INVALID_PARAMETERS - if client is NULL.
* 	CLIENT_MANAGRE_NOT_EXISTS - if client is not registered.
* 	CLIENT_MANAGRE_SUCCESS - in case of success.
*/
ClientsManagerCode clientsManagerRemove(ClientsManager manager, Client client);

/**
* clientsManagerGetClient: searches for client with the specified email address
*
* @param manager Target clients Manager to search in.
* @param email address to search. searches by string value and not by pointers.
*
* @return
* 	NULL if manager is NULL or client email not registered,
* 	else in case of success.
*/
Client clientsManagerGetClient(ClientsManager manager, char* email);



#endif /* SRC_CLIENTSMANAGER_H_ */
