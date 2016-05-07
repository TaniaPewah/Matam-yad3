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

static Agent agentsManagerGetAgent(AgentsManager manager, Email email);
static MapDataElement GetDataCopy(constMapDataElement data);
static MapKeyElement GetKeyCopy(constMapKeyElement key);
static void FreeData(MapDataElement data);
static void FreeKey(MapKeyElement key);
static int CompareKeys(constMapKeyElement first, constMapKeyElement second);
static void freeListElement(ListElement element);
static ListElement copyListElement(ListElement element);
static AgentsManagerResult convertAgentResult(AgentResult value);
static bool isPriceValid( int price );
static bool isValid( int param );


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
	AgentsManager manager = malloc(sizeof(*manager));
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
* @param Agent the new Agent email.
* @param Agent the new Agent company name.
* @param Agent the new Agent tax percentage.
*
* @return
* 	AGENT_MANAGER_INVALID_PARAMETERS - if Agent or manager is NULL .
* 	AGENT_MANAGER_ALREADY_EXISTS - if Agent email already registered.
* 	AGENT_MANAGER_OUT_OF_MEMORY - if failed to add the agent because of
* 									failed memory allocation
* 	AGENT_MANAGER_SUCCESS - in case of success.
*/
AgentsManagerResult agentsManagerAdd(AgentsManager manager, Email email,
		char* company_name, int tax_percentage) {
	if ((manager == NULL) || (email == NULL) || (company_name == NULL) ||
		(tax_percentage < 1) || (tax_percentage > 100))
		return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = NULL;
	AgentResult result = agentCreate(email, company_name, tax_percentage,
		&agent);
	if (result != AGENT_SUCCESS) {
		if (result == AGENT_OUT_OF_MEMORY) {
			return AGENT_MANAGER_OUT_OF_MEMORY;
		}
		else return AGENT_MANAGER_INVALID_PARAMETERS;
	}
	if (agentsManagerGetAgent(manager, email) != NULL) {
		agentDestroy(agent);
		return AGENT_MANAGER_ALREADY_EXISTS;
	}
	MapResult map_result = mapPut(manager->agentsMap, email, agent);
	agentDestroy(agent);
	if (map_result != MAP_SUCCESS) return AGENT_MANAGER_OUT_OF_MEMORY;
	return AGENT_MANAGER_SUCCESS;
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
* @param serviceName the new service name
* @param max_apartments the maximum number of apartments allowed in service
*
* @return
*
* 	AGENT_MANAGER_OUT_OF_MEMORY
* 		if memory allocation failed
*
*	AGENT_MANAGER_INVALID_PARAMETERS
*		if any of parameters are NULL or max_apartments
*
*	AGENT_MANAGER_ALREADY_EXISTS
*		if service with same name already exists
*
*	AGENT_MANAGER_AGENT_NOT_EXISTS
*	    if agent by this name does not exist
*
*	AGENT_MANAGER_SUCCESS
*	    if service successfully added
*/
AgentsManagerResult agentsManagerAddApartmentService(AgentsManager manager,
		Email email, char* serviceName, int max_apartments) {
	if((manager == NULL) || (email == NULL) || (serviceName == NULL) ||
			(max_apartments <= 0)) return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = agentsManagerGetAgent(manager, email);
	if(agent == NULL)
		return AGENT_MANAGER_AGENT_NOT_EXISTS;
	if(agentGetService(agent, serviceName))
		return AGENT_MANAGER_ALREADY_EXISTS;
	if (agentAddService(agent, serviceName, max_apartments) != AGENT_SUCCESS)
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
* @param manager 	  Target Agents Manager
* @param email   	  email of the requested agent
* @param service_name a name of the service to add the apartment to
* @param id	          the apartment id to add
* @param price	      the apartment price
* @param width	      the apartment width
* @param height	      the apartment height
* @param matrix	      the apartment shape, represented by a string of 'e'
* 					  and 'w'
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
	Email email, char* service_name, int id, int price, int width, int height,
	char* matrix) {
	if ((manager == NULL) || (email == NULL)|| (service_name == NULL) ||
		(id < 0)) return AGENT_MANAGER_INVALID_PARAMETERS;
	Agent agent = agentsManagerGetAgent(manager, email);
	if(agent == NULL) return AGENT_MANAGER_AGENT_NOT_EXISTS;
	AgentResult result = agentAddApartmentToService(agent, service_name, id,
			price, width, height, matrix);
	return convertAgentResult(result);
}

static AgentsManagerResult convertAgentResult(AgentResult value) {
	AgentsManagerResult result;
	switch(value) {
		case AGENT_INVALID_PARAMETERS : {
			result = AGENT_MANAGER_INVALID_PARAMETERS;
			break;
		}
		case AGENT_APARTMENT_SERVICE_NOT_EXISTS : {
			result = AGENT_MANAGER_SERVICE_NOT_EXISTS;
			break;
		}
		case AGENT_OUT_OF_MEMORY : {
			result = AGENT_MANAGER_OUT_OF_MEMORY;
			break;
		}
		case AGENT_APARTMENT_EXISTS: {
			result = AGENT_MANAGER_ALREADY_EXISTS;
			break;
		}
		case AGENT_APARTMENT_NOT_EXISTS : {
			result = AGENT_MANAGER_APARTMENT_NOT_EXISTS;
			break;
		}
		case AGENT_APARTMENT_SERVICE_FULL: {
			result = AGENT_MANAGER_APARTMENT_SERVICE_FULL;
			break;
		}
		default:
			result = AGENT_MANAGER_SUCCESS;
	}
	return result;
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
													!isValid( apartmentId ) )
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

	if ( manager == NULL || result_list == NULL || !isValid(min_area) ||
			!isValid(min_rooms) || !isPriceValid(max_price))
		return AGENT_MANAGER_INVALID_PARAMETERS;
	Email curr_email = mapGetFirst( manager->agentsMap );
	Agent curr_agent = NULL;
	if( curr_email != NULL)
		curr_agent = agentsManagerGetAgent( manager, curr_email);

	AgentDetails curr_details;

	List agents_list = listCreate(copyListElement, freeListElement);

	if(agents_list == NULL ) return AGENT_MANAGER_OUT_OF_MEMORY;

	while( curr_agent ){

		AgentResult result = agentFindMatch( curr_agent, min_area,
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

		curr_agent= NULL;
		curr_email = mapGetNext( manager->agentsMap );
		if( curr_email != NULL)
			curr_agent = agentsManagerGetAgent( manager, curr_email);
	}

	if( listGetSize( agents_list ) == 0 )
		return AGENT_MANAGER_APARTMENT_NOT_EXISTS;

	*result_list = agents_list;

	return AGENT_MANAGER_SUCCESS;
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

AgentsManagerResult getSignificantAgents( AgentsManager manager, int count ,
		List* significant_list ){
	if (isValid(count)){

		Agent curr_agent = NULL;
		Email curr_email = mapGetFirst(manager->agentsMap);
		if( curr_email != NULL)
			curr_agent = agentsManagerGetAgent( manager, curr_email);

		AgentDetails curr_details;
		List agents_list = listCreate(copyListElement, freeListElement);
		if(agents_list == NULL)
			return AGENT_MANAGER_OUT_OF_MEMORY;

		int rank;
		while( curr_agent ){

			rank = agentGetRank( curr_agent );
			if( rank != RANK_EMPTY ){

				curr_details = agentDetailsCreate( curr_email,
										agentGetCompany( curr_agent ), rank);
				if(curr_details == NULL)
					listDestroy( agents_list );
					return AGENT_MANAGER_OUT_OF_MEMORY;

				if ( listInsertLast(agents_list, curr_details) ==
						LIST_OUT_OF_MEMORY){
					listDestroy( agents_list );
					return AGENT_MANAGER_OUT_OF_MEMORY;
				}
			}

			curr_agent = NULL;
			curr_email = mapGetFirst(manager->agentsMap);
			if( curr_email != NULL)
				curr_agent = agentsManagerGetAgent( manager, curr_email);
		}

		*significant_list = agents_list;

		return AGENT_MANAGER_SUCCESS;
	}
	return AGENT_MANAGER_INVALID_PARAMETERS;
}


///* isValid: The function checks whether the given apartment numerical
// * 					param is valid
// *
// * @param  The param to check.
// *
// * * @return
// * false if invalid; else returns true.
// */
//static bool isValid( int param ){
//	return param > 0;
//}
//
///* priceisValid: The function checks whether the price gane be divided by 10
// *
// * @price  The price to check.
// *
// * * @return
// * false if invalid; else returns true.
// */
//static bool isPriceValid( int price ){
//	return !(price%10);
//}




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

