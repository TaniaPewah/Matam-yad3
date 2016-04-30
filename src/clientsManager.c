#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "clientsManager.h"
#include "client.h"
#include "string.h"
#include "map.h"

struct clientsManager_t {
	Map clientsMap;
};

static MapDataElement GetDataCopy(constMapDataElement data);
static MapKeyElement GetKeyCopy(constMapKeyElement key);
static void FreeData(MapDataElement data);
static void FreeKey(MapKeyElement key);
static int CompareKeys(constMapKeyElement first, constMapKeyElement second);

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
	if (clients == NULL) {
		free(clients);
		return NULL;
	} else {
		manager->clientsMap = clients;
		return manager;
	}
}

/** Function to be used for copying data elements into the map */
static MapDataElement GetDataCopy(constMapDataElement data) {
	return (Client)data;
}

/** Function to be used for copying key elements into the map */
static MapKeyElement GetKeyCopy(constMapKeyElement key) {
	return strdup((char*)key);
}

/** Function to be used for freeing data elements into the map */
static void FreeData(MapDataElement data) {
	clientDestroy((Client)data);
}

/** Function to be used for freeing key elements into the map */
static void FreeKey(MapKeyElement key) {
	free((char*)key);
}

/** Function to be used for comparing key elements in the map */
static int CompareKeys(constMapKeyElement first, constMapKeyElement second) {
	return strcmp((char*)first, (char*)second);
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
* 	CLIENT_MANAGRE_INVALID_PARAMETERS - if client is NULL.
* 	CLIENT_MANAGRE_ALREADY_EXISTS - if client email already registered.
* 	CLIENT_MANAGRE_OUT_OF_MEMORY - if failed allocating.
* 	CLIENT_MANAGRE_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerAdd(ClientsManager manager, Client client) {
	if (manager == NULL || client == NULL)
		return CLIENT_MANAGRE_INVALID_PARAMETERS;
	if (mapContains(manager->clientsMap, (constMapKeyElement)clientGetMail
			(client))) return CLIENT_MANAGRE_ALREADY_EXISTS;
	if (!mapPut(manager->clientsMap, (constMapKeyElement)clientGetMail(client),
				(constMapDataElement)client) != MAP_SUCCESS) {
		return CLIENT_MANAGRE_OUT_OF_MEMORY;
	} else {
		return CLIENT_MANAGRE_SUCCESS;
	}
}

/**
* clientsManagerRemove: removes the given client from the collection.
* note that the client will not be deallocated!
*
* @param manager Target clients Manager to remove from.
* @param email Target client email to remove.
*
* @return
* 	CLIENT_MANAGRE_INVALID_PARAMETERS - if client is NULL.
* 	CLIENT_MANAGRE_NOT_EXISTS - if client is not registered.
* 	CLIENT_MANAGRE_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerRemove(ClientsManager manager, char* email){
	if (manager == NULL || email == NULL)
		return CLIENT_MANAGRE_INVALID_PARAMETERS;
	if (!mapContains(manager->clientsMap, (constMapKeyElement)email))
		return CLIENT_MANAGRE_NOT_EXISTS;
	mapRemove(manager->clientsMap, (constMapKeyElement)email);
	return CLIENT_MANAGRE_SUCCESS;
}

/**
* clientsManagerGetClient: searches for client with the specified email address
*
* @param manager Target clients Manager to search in.
* @param email address to search. searches by string value and not by pointers.
* @param client pointer to save client in.
*
* @return
* 	CLIENT_MANAGRE_INVALID_PARAMETERS - if manager or email are NULL.
* 	CLIENT_MANAGRE_NOT_EXISTS - if client is not registered.
* 	CLIENT_MANAGRE_SUCCESS - in case of success.
*/
ClientsManagerResult clientsManagerGetClient(ClientsManager manager,
		char* email, Client* client) {
	if (manager == NULL || email == NULL)
		return CLIENT_MANAGRE_INVALID_PARAMETERS;
	if (!mapContains(manager->clientsMap, (constMapKeyElement)email))
		return CLIENT_MANAGRE_NOT_EXISTS;
	*client = (Client)mapGet(manager->clientsMap, (constMapKeyElement)email);
	return CLIENT_MANAGRE_SUCCESS;
}
