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
#include "agentDetails.h"
#include "map.h"
#include "list.h"

struct agentsManager_t {

	Map agentsMap;
};

AgentResult agentCopy(Agent agent, Agent* result);
static Agent agentsManagerGetAgent(AgentsManager manager, Email email);
static MapDataElement GetDataCopy(constMapDataElement data);
static MapKeyElement GetKeyCopy(constMapKeyElement key);
static void FreeData(MapDataElement data);
static void FreeKey(MapKeyElement key);
static int CompareKeys(constMapKeyElement first, constMapKeyElement second);
static void freeListElement(ListElement element);
static ListElement copyListElement(ListElement element);

/**
* Allocates a new AgentsManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new AgentsManager in case of success.
*/
AgentsManager agentsManagerCreate(){

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
* agentsManagerDestroy: Deallocates an existing manager.
* Clears the element by using the stored free function.
*
* @param manager Target Agents Manager to be deallocated.
* If manager is NULL nothing will be done
*/
void agentsManagerDestroy(AgentsManager manager){

	if (manager != NULL) {
		mapDestroy(manager->agentsMap);
		free(manager);
	}
}

/**
* agentsManagerAddAgent: adds the new Agent to the collection.
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
AgentsManagerResult agentsManagerAdd(AgentsManager manager, Agent agent){
	if(manager == NULL || agent == NULL)
		return AGENT_MANAGER_INVALID_PARAMETERS;
	if( agentsManagerGetAgent(manager, agentGetMail(agent)) != NULL )
		return AGENT_MANAGER_ALREADY_EXISTS;
	if ( mapPut(manager->agentsMap, agentGetMail(agent),
			agent) != MAP_SUCCESS ) {
		return AGENT_MANAGER_OUT_OF_MEMORY;
	} else {
		return AGENT_MANAGER_SUCCESS;
	}
}

/**
* agentsManagerRemove: removes the given Agent from the collection.
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
AgentsManagerResult agentsManagerRemove(AgentsManager manager, Email email){

	if( manager == NULL || email == NULL )
		return AGENT_MANAGER_INVALID_PARAMETERS;
	if( agentsManagerGetAgent( manager, email) == NULL )
		return AGENT_MANAGER_AGENT_NOT_EXISTS;
	mapRemove( manager->agentsMap, (constMapKeyElement)email);
	return AGENT_MANAGER_SUCCESS;
}

/**
* agentsManagerGetAgent: searches for Agent with the specified email address
*
* @param manager Target Agents Manager to search in.
* @param email address to search. searches by string value and not by pointers.
*
* @return
* 	NULL if manager is NULL or Agent email not registered,
* 	the agent in case of success.
*/
static Agent agentsManagerGetAgent(AgentsManager manager, Email email){

	Agent agent = NULL;
	if( manager != NULL && email != NULL ){
		agent = mapGet( manager->agentsMap, email);
	}
	return agent;
}

/**
* agentManagerAddApartmentService: add apartment service to requested agent
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
AgentsManagerResult agentsManagerAddApartmentService(AgentsManager manager,
		Email email, ApartmentService apartmentService, char* serviceName){

	if( manager == NULL || email == NULL || apartmentService == NULL )
		return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = agentsManagerGetAgent( manager, email );
	if( agent == NULL )
		return AGENT_MANAGER_AGENT_NOT_EXISTS;
	if( agentGetService(agent, serviceName))
		return AGENT_MANAGER_ALREADY_EXISTS;
	if (agentAddService( agent, apartmentService, serviceName ) !=
																AGENT_SUCCESS)
		return AGENT_MANAGER_OUT_OF_MEMORY;
	else
		return AGENT_MANAGER_SUCCESS;
}

/**
* agentManagerRemoveApartmentService: remove apartment service
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
AgentsManagerResult agentsManagerRemoveApartmentService(AgentsManager manager,
										Email email, char* serviceName){

	if( manager == NULL || email == NULL || serviceName == NULL )
			return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = agentsManagerGetAgent( manager, email);
		if( agent == NULL )
			return AGENT_MANAGER_AGENT_NOT_EXISTS;

	AgentResult result = agentRemoveService( agent, serviceName);

	if( result == AGENT_INVALID_PARAMETERS )
		return AGENT_MANAGER_INVALID_PARAMETERS;

	if( result == AGENT_APARTMENT_SERVICE_NOT_EXISTS )
		return AGENT_MANAGER_SERVICE_NOT_EXISTS;

	return AGENT_MANAGER_SUCCESS;
}

/**
* agentManagerAddApartmentToService: add apartment to apartment service
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
AgentsManagerResult agentsManagerAddApartmentToService(AgentsManager manager,
				Email email, char* serviceName, Apartment apartment, int id ){

	if( manager == NULL || email == NULL ||	apartment == NULL ||
															!idIsValid( id ))
		return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = agentsManagerGetAgent( manager, email);
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
* agentManagerRmoveApartmentFromService: add apartment to apartment service
* 									  of requested agent
* @param manager 	 Target Agents Manager
* @param email   	 email of the requested agent
* @param serviceName a name of the service to remove the apartment from
* @param apartmentId the id of apartment to remove
*
* @return
*	AGENT_MANAGER_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGER_AGENT_NOT_EXISTS       if agent by this name doesnt exist
*	AGENT_MANAGER_APARTMENT_NOT_EXISTS apartment with the given id doesnt exist
*	AGENT_MANAGER_SERVICE_NOT_EXISTS	service does not exist
*	AGENT_MANAGER_SUCCESS              if apartment successfully removed
*/
AgentsManagerResult agentsManagerRemoveApartmentFromService(
	AgentsManager manager, Email email, char* serviceName, int apartmentId ){
	if( manager == NULL || email == NULL || serviceName == NULL ||
												!idIsValid( apartmentId ) )
		return AGENT_MANAGER_INVALID_PARAMETERS;

	Agent agent = agentsManagerGetAgent( manager, email);

	if( agent == NULL )
		return AGENT_MANAGER_AGENT_NOT_EXISTS;

	AgentResult result = agentRemoveApartmentFromService(
											agent, apartmentId, serviceName );
	switch( result ){
		case AGENT_INVALID_PARAMETERS:{
			return AGENT_MANAGER_INVALID_PARAMETERS;
			break;
		}
		case AGENT_APARTMENT_NOT_EXISTS:{
			return AGENT_MANAGER_APARTMENT_NOT_EXISTS;
			break;
		}
		case AGENT_APARTMENT_SERVICE_NOT_EXISTS:{
			return AGENT_MANAGER_SERVICE_NOT_EXISTS;
			break;
		}
		default:{
			break;
		}
	}
	return AGENT_MANAGER_SUCCESS;
}

/**
* agentFindMatch: finds a matching apartment in each of the agent's services
*
* @param agent   	the requested agent
* @param min_rooms  the minimum amounts of rooms in the requested apartment
* @param min_area   the minimum area in the requested apartment
* @param max_price  the maximum price of the apartment
* @param details out parameter AgentDetails array with the details of the
* 				 agents who has the matching apartments
*
* @return
*	AGENT_MANAGER_APARTMENT_NOT_EXISTS  if the matching apartment is not found
*	AGENT_OUT_OF_MEMORY                 if any of the allocations failed
*	AGENT_MANAER_SUCCESS                at least one match is found
*/
AgentsManagerResult agentManagerFindMatch( AgentsManager manager, int min_rooms,
					 int min_area, int max_price, List* result_list ){

	if (manager == NULL || result_list == NULL)
		return AGENT_MANAGER_INVALID_PARAMETERS;
	AgentsManagerResult manager_result = AGENT_MANAGER_SUCCESS;
	Agent current = mapGetFirst( manager->agentsMap );

	if( current == NULL)
		manager_result = AGENT_MANAGER_AGENT_NOT_EXISTS;
	AgentDetails curr_details;

	List agents_list = listCreate(copyListElement, freeListElement);

	if(agents_list == NULL ) return AGENT_MANAGER_OUT_OF_MEMORY;

	while( current ){

		AgentResult result = agentFindMatch( current, min_area,
										min_rooms, max_price, &curr_details);
		if( result == AGENT_OUT_OF_MEMORY ){
			listDestroy(agents_list);
			return AGENT_MANAGER_OUT_OF_MEMORY;
		}

		if( result == AGENT_SUCCESS &&
			listInsertLast(agents_list, (ListElement)(curr_details)) ==
															LIST_OUT_OF_MEMORY){
				listDestroy(agents_list);
				return AGENT_MANAGER_OUT_OF_MEMORY;
			}

		current = mapGetNext( manager->agentsMap );
	}

	if( listGetSize( agents_list ) > 0 ){
		manager_result = AGENT_MANAGER_SUCCESS;
	}

	*result_list = agents_list;

	return manager_result;
}

/* agentsManagerAgentExists: The function checks whether there is an agent
 * registered under the given e-mail
 *
 * @param manager Target agent Manager to search in.
 * @param email address to search agent by.
 *
 * * @return
 * false if one of the parameters is NULL or if the agent does not exist in
 * the managers collection; else if agent exists returns true.
 */
bool agentsManagerAgentExists(AgentsManager manager, Email email){
	if ((manager == NULL) || (email == NULL)) return false;
	return mapContains(manager->agentsMap, email);
}

bool idIsValid( int id ){
	return id > 0;
}

/** Function to be used for copying data elements into the map */
static MapDataElement GetDataCopy(constMapDataElement data) {
	Agent new_agent = NULL;
	agentCopy((Agent)data, &new_agent);
	return (MapDataElement)new_agent;
}

/** Function to be used for copying key elements into the map */
static MapKeyElement GetKeyCopy(constMapKeyElement key) {
	Email new_email = NULL;
	emailCopy((Email)key, &new_email);
	return new_email;
}

/** Function to be used for freeing data elements into the map */
static void FreeData(MapDataElement data) {
	if (data != NULL) agentDestroy((Agent)data);
}

/** Function to be used for freeing key elements into the map */
static void FreeKey(MapKeyElement key) {
	if (key != NULL) emailDestroy((Email)key);
}

/** Function to be used for comparing key elements in the map */
static int CompareKeys(constMapKeyElement first, constMapKeyElement second) {
	return emailComapre((Email)first, (Email)second);
}

/** Function to be used for freeing data elements from list */
void freeListElement(ListElement element) {
	if (element != NULL)
		agentDetailsDestroy((AgentDetails)element);
}

/** Function to be used for coping data elements from list */
ListElement copyListElement(ListElement element) {
	AgentDetails agent_details =
			agentDetailsCopy((AgentDetails)element);
	return agent_details;
}

