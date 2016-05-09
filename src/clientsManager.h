#ifndef SRC_CLIENTSMANAGER_H_
#define SRC_CLIENTSMANAGER_H_

#include "client.h"
#include "email.h"
#include "list.h"

/**
* This type defines end codes for the methods.
*/
typedef enum {
	CLIENT_MANAGER_OUT_OF_MEMORY = 0,
	CLIENT_MANAGER_NULL_PARAMETERS = 1,
	CLIENT_MANAGER_INVALID_PARAMETERS = 2,
	CLIENT_MANAGER_ALREADY_EXISTS = 3,
	CLIENT_MANAGER_NOT_EXISTS = 4,
	CLIENT_MANAGER_SUCCESS = 5
} ClientsManagerResult;

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
* @param email clients email.
* @param apartment_min_area minimal area for the clients wanted apartments
* @param apartment_min_rooms minimal room count in clients wanted apartments
* @param apartment_max_price maximum price for the clients wanted apartments
*
* @return
* 	CLIENT_MANAGER_NULL_PARAMETERS - if manager or email is NULL.
*
* 	CLIENT_MANAGER_ALREADY_EXISTS - if client email already registered.
*
* 	CLIENT_MANAGER_INVALID_PARAMETERS -  if apartment_min_area,
* 		apartment_min_rooms or apartment_max_price are not bigger then zero.
*
* 	CLIENT_MANAGER_OUT_OF_MEMORY - if failed allocating.
*
* 	CLIENT_MANAGER_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerAdd(ClientsManager manager, Email email,
		int apartment_min_area, int apartment_min_rooms,
		int apartment_max_price);

/**
* clientsManagerRemove: removes the given client from the collection.
* note that the client will be deallocated!
*
* @param manager Target clients Manager to remove from.
* @param email Target client email to remove.
*
* @return
* 	CLIENT_MANAGER_INVALID_PARAMETERS - if client is NULL.
* 	CLIENT_MANAGER_NOT_EXISTS - if client is not registered.
* 	CLIENT_MANAGER_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerRemove(ClientsManager manager, Email email);

/* clientsManagerClientExists: The function checks whether there is a customer
 * registered under the given e-mail
 *
 * @param manager Target clients Manager to search in.
 * @param email address to search client by.
 *
 * * @return
 * false if one of the parameters is NULL or if the client does not exist in
 * the managers collection; else if client exists returns true.
 */
bool clientsManagerClientExists(ClientsManager manager, Email email);

/**
* clientsManagerGetRestriction: finds the clients restriction for apartments.
*
* @param manager Target clients Manager.
* @param email clients email to find to.
* @param apartment_min_area pointer to store minimal area for the clients
* 	wanted apartments
* @param apartment_min_rooms pointer to store minimal room count in clients
* 	wanted apartments
* @param apartment_max_price pointer to store maximum price for the clients
* 	wanted apartments
*
* @return
* 	CLIENT_MANAGER_NULL_PARAMETERS - if manager, email or one of the pointers
* 		are NULL.
*
* 	CLIENT_MANAGER_NOT_EXISTS - if client email is not registered.
*
* 	CLIENT_MANAGER_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerGetRestriction(ClientsManager manager,
		Email email, int* apartment_min_area, int* apartment_min_rooms,
		int* apartment_max_price);

/**
* clientsManagerExecutePurchase: commits an apartment purchase for a given
* client.
*
* @param manager Target clients Manager.
* @param email clients email to find to.
* @param finalPrice the apartment final price, for the client to pay
*
* @return
* 	CLIENT_MANAGER_NULL_PARAMETERS - if manager or email are NULL.
*
* 	CLIENT_MANAGER_NOT_EXISTS - if client email is not registered.
*
* 	CLIENT_MANAGER_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerExecutePurchase(ClientsManager manager,
		Email mail, int finalPrice);

/**
* clientsManagerGetSortedPayments: creates a list of all the registered clients
* who bought apartments, sorted firstly by the payment size, and secondly
* Alphabetically by the email
*
* @param manager Target clients Manager to use.
* @param list pointer to save destination list in.
*
* @return
* 	CLIENT_MANAGER_INVALID_PARAMETERS - if manager or list are NULL.
* 	CLIENT_MANAGER_OUT_OF_MEMORY - in case of allocation failure.
* 	CLIENT_MANAGER_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerGetSortedPayments(ClientsManager manager,
		List* list);

#endif /* SRC_CLIENTSMANAGER_H_ */
