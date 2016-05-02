/*
 * agentsManager.c
 *
 *  Created on: Apr 30, 2016
 *      Author: Tania
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "agentsManager.h"
#include "agent.h"
#include "map.h"

struct agentsManager_t {

	Map agentsMap;
};

static MapDataElement GetDataCopy(constMapDataElement data);
static MapKeyElement GetKeyCopy(constMapKeyElement key);
static void FreeData(MapDataElement data);
static void FreeKey(MapKeyElement key);
static int CompareKeys(constMapKeyElement first, constMapKeyElement second);

/**
* Allocates a new AgentsManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new AgentsManager in case of success.
*/
AgentsManager AgentsManagerCreate(){

	Map agents = mapCreate(GetDataCopy, GetKeyCopy, FreeData, FreeKey,
				CompareKeys);
	if (agents == NULL) return NULL;
	AgentsManager manager = malloc (sizeof(*manager));
	if (manager == NULL) {
		free(agents);
		return NULL;
	} else {
		manager->agentsMap = agents;
		return manager;
	}
}

/**
* AgentsManagerDestroy: Deallocates an existing manager.
* Clears the element by using the stored free function.
*
* @param manager Target Agents Manager to be deallocated.
* If manager is NULL nothing will be done
*/
void AgentsManagerDestroy(AgentsManager manager){

	if (manager != NULL) {
		mapDestroy(manager->agentsMap);
		free(manager);
	}
}

/**
* AgentsManagerAddAgent: adds the new Agent to the collection.
*
* @param manager Target Agents Manager to add to.
* @param Agent Target Agent to add.
*
* @return
* 	AGENT_MANAGER_INVALID_PARAMETERS - if Agent or manager is NULL .
* 	AGENT_MANAGER_ALREADY_EXISTS - if Agent email already registered.
* 	AGENT_MANAGER_SUCCESS - in case of success.
*//*
AgentsManagerResult AgentsManagerAdd(AgentsManager manager, Agent agent){
	if(manager == NULL || agent == NULL){
		return AGENT_MANAGER_INVALID_PARAMETERS;
	} else if(AgentsManagerGetAgent(manager, agent->email)){
		return AGENT_MANAGER_ALREADY_EXISTS;
	} else {
		addAgent(manager->agentsMap, agent);
		return AGENT_MANAGER_SUCCESS;
	}
}*/

/**
* AgentsManagerRemove: removes the given Agent from the collection.
* note that the Agent will not be deallocated!
*
* @param manager Target Agents Manager to remove from.
* @param Agent Target Agent to remove.
*
* @return
* 	AGENT_MANAGER_INVALID_PARAMETERS - if Agent is NULL.
* 	AGENT_MANAGER_NOT_EXISTS - if Agent is not registered.
* 	AGENT_MANAGER_SUCCESS - in case of success.
*/
AgentsManagerResult AgentsManagerRemove(AgentsManager manager, Agent Agent);

/**
* AgentsManagerGetAgent: searches for Agent with the specified email address
*
* @param manager Target Agents Manager to search in.
* @param email address to search. searches by string value and not by pointers.
*
* @return
* 	NULL if manager is NULL or Agent email not registered,
* 	the agent in case of success.
*/
Agent AgentsManagerGetAgent(AgentsManager manager, char* email){

	return NULL;
}

/**
* AgentManagerAddApartmentService: add apartment service to requested agent
*
* @param manager Target Agents Manager
* @param email  email of the requested agent
* @param appartmentService a pointer to a service to add to the agent
*
* @return
*
* 	AGENT_MANAGER_OUT_OF_MEMORY 	if memory allocation failed
*	AGENT_MANAGER_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGER_ALREADY_EXISTS       if service with same name already exists
*	AGENT_MANAGER_NOT_EXISTS           if agent by this name does not exist
*	AGENT_MANAGER_SUCCESS              if service successfully added
*/
AgentsManagerResult AgentManagerAddApartmentService(AgentsManager manager,
										char* email, void* apartmentService);

/**
* AgentManagerRemoveApartmentService: remove apartment service
* 									  from requested agent
* @param manager 	 Target Agents Manager
* @param email   	 email of the requested agent
* @param serviceName a name of the service to remove
*
* @return
*	AGENT_MANAGER_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGER_NOT_EXISTS           if agent by this name does not exist or
*									service with the given name does not exist
*	AGENT_MANAGER_SUCCESS              if service successfully removed
*/
AgentsManagerResult AgentManagerRemoveApartmentService(AgentsManager manager,
										char* email, char* serviceName);

/**
* AgentManagerAddApartmentToService: add apartment to apartment service
* 									  of requested agent
* @param manager 	 Target Agents Manager
* @param email   	 email of the requested agent
* @param serviceName a name of the service to add the apartment to
* @param apartment	 the apartment to add
*
* @return
*	AGENT_MANAGER_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGER_NOT_EXISTS           if agent by this name does not exist or
*									service with the given name does not exist
*	AGENT_MANAGER_ALREADY_EXISTS       if apartment with the given id
*									already exist
*	AGENT_MANAGER_SUCCESS              if apartment successfully added
*/
AgentsManagerResult AgentManagerAddApartmentToService(AgentsManager manager,
							char* email, char* serviceName, void* apartment);


/**
* AgentManagerAddApartmentToService: add apartment to apartment service
* 									  of requested agent
* @param manager 	 Target Agents Manager
* @param email   	 email of the requested agent
* @param serviceName a name of the service to remove the apartment from
* @param apartmentId the id of apartment to remove
*
* @return
*	AGENT_MANAGER_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGER_NOT_EXISTS           if agent by this name does not exist or
*									service with the given name does not exist
*									or apartment does not exist
*	AGENT_MANAGER_SUCCESS              if apartment successfully removed
*/
AgentsManagerResult AgentManagerRemoveApartmentFromService(AgentsManager manager,
							char* email, char* serviceName, int apartmentId );

/** Function to be used for copying data elements into the map */
static MapDataElement GetDataCopy(constMapDataElement data) {
	return (Agent)data;
}

/** Function to be used for copying key elements into the map */
static MapKeyElement GetKeyCopy(constMapKeyElement key) {
	//return strdup((char*)key);
	return NULL;
}

/** Function to be used for freeing data elements into the map */
static void FreeData(MapDataElement data) {
	//agentDestroy((Agent)data);
}

/** Function to be used for freeing key elements into the map */
static void FreeKey(MapKeyElement key) {
	free((char*)key);
}

/** Function to be used for comparing key elements in the map */
static int CompareKeys(constMapKeyElement first, constMapKeyElement second) {
	return strcmp((char*)first, (char*)second);
}


