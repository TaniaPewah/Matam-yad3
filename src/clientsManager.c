#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "clientPurchaseBill.h"
#include "clientsManager.h"
#include "client.h"
#include "email.h"
#include "list.h"
#include "map.h"

struct clientsManager_t {
	Map clientsMap;
};

static MapDataElement GetDataCopy(constMapDataElement data);
static MapKeyElement GetKeyCopy(constMapKeyElement key);
static void FreeData(MapDataElement data);
static void FreeKey(MapKeyElement key);
static int CompareKeys(constMapKeyElement first, constMapKeyElement second);
static void freeListElement(ListElement element);
static ListElement copyListElement(ListElement element);
static int compareListElements(ListElement first, ListElement second);

/**
* Allocates a new ClientsManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new clients in case of success.
*/
ClientsManager clientsManagerCreate() {
	Map clients = mapCreate(GetDataCopy, GetKeyCopy, FreeData, FreeKey,
			CompareKeys);
	if (clients == NULL) return NULL;
	ClientsManager manager = malloc (sizeof(*manager));
	if (manager == NULL) {
		free(clients);
		return NULL;
	} else {
		manager->clientsMap = clients;
		return manager;
	}
}

/** Function to be used for copying data elements into the map */
static MapDataElement GetDataCopy(constMapDataElement data) {
	Client new_client = NULL;
	clientCopy((Client)data, &new_client);
	return (MapDataElement)new_client;
}

/** Function to be used for copying key elements into the map */
static MapKeyElement GetKeyCopy(constMapKeyElement key) {
	Email new_email = NULL;
	emailCopy((Email)key, &new_email);
	return (MapKeyElement)new_email;
}

/** Function to be used for freeing data elements into the map */
static void FreeData(MapDataElement data) {
	if (data != NULL) clientDestroy((Client)data);
}

/** Function to be used for freeing key elements into the map */
static void FreeKey(MapKeyElement key) {
	if (key != NULL) emailDestroy((Email)key);
}

/** Function to be used for comparing key elements in the map */
static int CompareKeys(constMapKeyElement first, constMapKeyElement second) {
	return emailComapre((Email)first, (Email)second);
}

/**
* clientsManagerDestroy: Deallocates an existing manager.
* Clears the element by using the stored free function.
*
* @param manager Target clients Manager to be deallocated.
* If manager is NULL nothing will be done
*/
void clientsManagerDestroy(ClientsManager manager) {
	if (manager != NULL) {
		mapDestroy(manager->clientsMap);
		free(manager);
	}
}

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
		int apartment_max_price) {
	if (manager == NULL || email == NULL)
		return CLIENT_MANAGER_NULL_PARAMETERS;
	Client client = NULL;
	ClientResult result = clientCreate(email, apartment_min_area,
			apartment_min_rooms, apartment_max_price, &client);
	if (result != CLIENT_SUCCESS){
		if (result == CLIENT_INVALID_PARAMETERS)
			return CLIENT_MANAGER_INVALID_PARAMETERS;
		return CLIENT_MANAGER_OUT_OF_MEMORY;
	}
	ClientsManagerResult manager_result;
	if (mapContains(manager->clientsMap, email)) {
		manager_result = CLIENT_MANAGER_ALREADY_EXISTS;
	} else if (!(mapPut(manager->clientsMap, (constMapKeyElement)email,
			(constMapDataElement)client) != MAP_SUCCESS)) {
		manager_result =  CLIENT_MANAGER_OUT_OF_MEMORY;
	} else {
		manager_result = CLIENT_MANAGER_SUCCESS;
	}
	clientDestroy(client);
	return manager_result;
}

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
ClientsManagerResult clientsManagerRemove(ClientsManager manager, Email email){
	if (manager == NULL || email == NULL)
		return CLIENT_MANAGER_INVALID_PARAMETERS;
	if (!mapContains(manager->clientsMap, (constMapKeyElement)email))
		return CLIENT_MANAGER_NOT_EXISTS;
	mapRemove(manager->clientsMap, (constMapKeyElement)email);
	return CLIENT_MANAGER_SUCCESS;
}

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
bool clientsManagerClientExists(ClientsManager manager, Email email) {
	if ((manager == NULL) || (email == NULL)) return false;
	return mapContains(manager->clientsMap, email);
}

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
		List* list) {
	if (manager == NULL || list == NULL)
		return CLIENT_MANAGER_INVALID_PARAMETERS;
	List new_list = listCreate(copyListElement, freeListElement);
	if (new_list == NULL) return CLIENT_MANAGER_OUT_OF_MEMORY;
	bool error = false;
	MapDataElement element = mapGetFirst(manager->clientsMap);
	while (element != NULL && !error) {
		Client client = (Client)mapGet(manager->clientsMap, element);
		if (clientGetTotalPayments(client) > 0) {
			ClientPurchaseBill bill = clientPurchaseBillCreate
					(clientGetMail(client), clientGetTotalPayments(client));
			error = (bill == NULL) ||
					(listInsertLast(new_list, (ListElement)(bill))
					!= LIST_SUCCESS);
		}
		element = mapGetNext(manager->clientsMap);
	}
	if ((error) || (listSort(new_list, compareListElements) != LIST_SUCCESS)) {
		listDestroy(new_list);
		return CLIENT_MANAGER_OUT_OF_MEMORY;
	}
	*list = new_list;
	return CLIENT_MANAGER_SUCCESS;
}

/** Function to be used for freeing data elements from list */
static void freeListElement(ListElement element) {
	if (element != NULL)
		clientPurchaseBillDestroy((ClientPurchaseBill)element);
}

/** Function to be used for coping data elements from list */
static ListElement copyListElement(ListElement element) {
	ClientPurchaseBill new_bill =
			clientPurchaseBillCopy((ClientPurchaseBill)element);
	return new_bill;
}

/** Function to be used for comparing data elements in the list */
static int compareListElements(ListElement first, ListElement second) {
	return clientPurchaseBillComapre((ClientPurchaseBill)first,
			(ClientPurchaseBill)second);
}

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
		int* apartment_max_price) {
	if ((manager == NULL) || (email == NULL) || (apartment_min_area ==  NULL)
		||  (apartment_min_rooms ==  NULL) ||  (apartment_max_price ==  NULL))
		return CLIENT_MANAGER_NULL_PARAMETERS;
	Client client = mapGet(manager->clientsMap, email);
	if (client == NULL) return CLIENT_MANAGER_NOT_EXISTS;
	*apartment_min_area = clientGetMinArea(client);
	*apartment_min_rooms = clientGetMinRooms(client);
	*apartment_max_price = clientGetMaxPrice(client);
	return CLIENT_MANAGER_SUCCESS;
}
