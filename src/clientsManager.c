#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "map.h"
#include "list.h"
#include "clientsManager.h"
#include "client.h"
#include "email.h"


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
	clientDestroy((Client)data);
}

/** Function to be used for freeing key elements into the map */
static void FreeKey(MapKeyElement key) {
	emailDestroy((Email)key);
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
* @param client Target client to add.
*
* @return
* 	CLIENT_MANAGER_INVALID_PARAMETERS - if client is NULL.
* 	CLIENT_MANAGER_ALREADY_EXISTS - if client email already registered.
* 	CLIENT_MANAGER_OUT_OF_MEMORY - if failed allocating.
* 	CLIENT_MANAGER_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerAdd(ClientsManager manager, Client client) {
	if (manager == NULL || client == NULL)
		return CLIENT_MANAGER_INVALID_PARAMETERS;
	if (mapContains(manager->clientsMap, (constMapKeyElement)clientGetMail
			(client))) return CLIENT_MANAGER_ALREADY_EXISTS;
	if (!mapPut(manager->clientsMap, (constMapKeyElement)clientGetMail(client),
				(constMapDataElement)client) != MAP_SUCCESS) {
		return CLIENT_MANAGER_OUT_OF_MEMORY;
	} else {
		return CLIENT_MANAGER_SUCCESS;
	}
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

/**
* clientsManagerGetClient: searches for client with the specified email address
*
* @param manager Target clients Manager to search in.
* @param email address to search. searches by string value and not by pointers.
* @param client pointer to save client in.
*
* @return
* 	CLIENT_MANAGER_INVALID_PARAMETERS - if manager or email are NULL.
* 	CLIENT_MANAGER_NOT_EXISTS - if client is not registered.
* 	CLIENT_MANAGER_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerGetClient(ClientsManager manager,
		Email email, Client* client) {
	if (manager == NULL || email == NULL)
		return CLIENT_MANAGER_INVALID_PARAMETERS;
	if (!mapContains(manager->clientsMap, (constMapKeyElement)email))
		return CLIENT_MANAGER_NOT_EXISTS;
	*client = (Client)mapGet(manager->clientsMap, (constMapKeyElement)email);
	return CLIENT_MANAGER_SUCCESS;
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
			error = listInsertLast(new_list, (ListElement)(client))
					!= LIST_SUCCESS;
		}
		element = mapGetNext(manager->clientsMap);
	}
	if (error) {
		listDestroy(new_list);
		return CLIENT_MANAGER_OUT_OF_MEMORY;
	} else {
		listSort(new_list, compareListElements);
		return CLIENT_MANAGER_SUCCESS;
	}
}

/** Function to be used for freeing data elements from list */
void freeListElement(ListElement element) {
	// Do nothing, don't deallocate the client! still using it in the map!
}

/** Function to be used for coping data elements from list */
ListElement copyListElement(ListElement element) {
	return (Client)element; // Don't copy, use the same client!
}

/** Function to be used for comparing data elements in the list */
static int compareListElements(ListElement first, ListElement second) {
	int diff = clientGetTotalPayments((Client)first) -
			 clientGetTotalPayments((Client)second);
	if (diff == 0) {
		diff = emailComapre(
			clientGetMail((Client)first), clientGetMail((Client)second));
	}
	return diff;
}
