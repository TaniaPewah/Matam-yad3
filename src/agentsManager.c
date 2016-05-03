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
* 	AGENT_MANAGER_OUT_OF_MEMORY - if failed to add the agent because of
* 									failed memory allocation
* 	AGENT_MANAGER_SUCCESS - in case of success.
*/
AgentsManagerResult AgentsManagerAdd(AgentsManager manager, Agent agent){
	if(manager == NULL || agent == NULL)
		return AGENT_MANAGER_INVALID_PARAMETERS;
	if( !AgentsManagerGetAgent(manager, agentGetMail(agent)))
		return AGENT_MANAGER_ALREADY_EXISTS;
	if (!mapPut(manager->agentsMap, (constMapKeyElement)agentGetMail(agent),
				(constMapDataElement)agent) != MAP_SUCCESS) {
		return AGENT_MANAGER_OUT_OF_MEMORY;
	} else {
		return AGENT_MANAGER_SUCCESS;
	}
}

/**
* AgentsManagerRemove: removes the given Agent from the collection.
* note that the Agent will not be deallocated!
*
* @param manager Target Agents Manager to remove from.
* @param email email of target Agent to remove.
*
* @return
* 	AGENT_MANAGER_INVALID_PARAMETERS - if Agent is NULL.
* 	AGENT_MANAGER_AGENT_NOT_EXISTS - if Agent is not registered.
* 	AGENT_MANAGER_SUCCESS - in case of success.
*/
AgentsManagerResult AgentsManagerRemove(AgentsManager manager, char* email){

	if( manager == NULL || email == NULL )
		return AGENT_MANAGER_INVALID_PARAMETERS;
	if( !AgentsManagerGetAgent( manager, email))
		return AGENT_MANAGER_AGENT_NOT_EXISTS;
	mapRemove( manager->agentsMap, (constMapKeyElement)email);
	return AGENT_MANAGER_SUCCESS;
}

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

	Agent agent = NULL;
	if( manager != NULL && email != NULL ){
		agent = mapGet( manager->agentsMap, (constMapKeyElement)email );
	}
	return agent;
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
* 	AGENT_MANAGER_OUT_OF_MEMORY 	   if memory allocation failed
*	AGENT_MANAGER_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGER_ALREADY_EXISTS       if service with same name already exists
*	AGENT_MANAGER_AGENT_NOT_EXISTS           if agent by this name does not exist
*	AGENT_MANAGER_SUCCESS              if service successfully added
*/
AgentsManagerResult AgentManagerAddApartmentService(AgentsManager manager,
					char* email, void* apartmentService, char* serviceName){

	if( manager == NULL || email == NULL || apartmentService )
		return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = AgentsManagerGetAgent( manager, email );
	if( !agent)
		return AGENT_MANAGER_AGENT_NOT_EXISTS;
	if( agentGetService(agent, apartmentService))
		return AGENT_MANAGER_ALREADY_EXISTS;
	if (agentAddService( agent, apartmentService, serviceName ) != AGENT_SUCCESS)
		return AGENT_MANAGER_OUT_OF_MEMORY;
	else
		return AGENT_MANAGER_SUCCESS;
}

/**
* AgentManagerRemoveApartmentService: remove apartment service
* 									  from requested agent
* @param manager 	 Target Agents Manager
* @param email   	 email of the requested agent
* @param serviceName a name of the service to remove
*
* @return
*	AGENT_MANAGER_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGER_AGENT_NOT_EXISTS     if agent by this name does not exist
*	AGENT_MANAGER_SERVICE_NOT_EXISTS if service with the given name doesnt exist
*	AGENT_MANAGER_SUCCESS              if service successfully removed
*/
AgentsManagerResult AgentManagerRemoveApartmentService(AgentsManager manager,
										char* email, char* serviceName){

	if( manager == NULL || email == NULL )
			return AGENT_MANAGER_INVALID_PARAMETERS;
		if( !AgentsManagerGetAgent( manager, email))
			return AGENT_MANAGER_AGENT_NOT_EXISTS;
		mapRemove( manager->agentsMap, (constMapKeyElement)email);
		return AGENT_MANAGER_SUCCESS;
}

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
*	AGENT_MANAGER_AGENT_NOT_EXISTS    if agent by this name does not exist
*	AGENT_MANAGER_OUT_OF_MEMORY		  if allocation failed
*	AGENT_MANAGER_SERVICE_NOT_EXISTS service with the given name does not exist
*	AGENT_MANAGER_ALREADY_EXISTS   if apartment with the given id already exist
*	AGENT_MANAGER_SERVICE_FULL	   if apartment service is full
*	AGENT_MANAGER_SUCCESS          if apartment successfully added
*/
AgentsManagerResult AgentManagerAddApartmentToService(AgentsManager manager,
				char* email, char* serviceName, Apartment apartment, int id ){

	if( manager == NULL || email == NULL ||	apartment == NULL )
		return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = AgentsManagerGetAgent( manager, email);
	if( agent == NULL )
		return AGENT_MANAGER_AGENT_NOT_EXISTS;

	AgentResult result = agentAddApartmentToService( agent, apartment, id,
													  serviceName );
	switch( result ){
		case AGENT_APARTMENT_SERVICE_NOT_EXISTS:{
			return AGENT_MANAGER_SERVICE_NOT_EXISTS;
			break;
		}
		case AGENT_OUT_OF_MEMORY : {
			return AGENT_MANAGER_OUT_OF_MEMORY;
			break;
		}
		case AGENT_APARTMENT_EXISTS: {
			return AGENT_MANAGER_ALREADY_EXISTS;
			break;
		}
		case AGENT_APARTMENT_SERVICE_FULL: {
			return AGENT_MANAGER_APARTMENT_SERVICE_FULL;
			break;
		}
		default:
		return AGENT_MANAGER_SUCCESS;
	}
}

/**
* AgentManagerRmoveApartmentFromService: add apartment to apartment service
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
							char* email, char* serviceName, int apartmentId ){

	return AGENT_MANAGER_SUCCESS;
}

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





