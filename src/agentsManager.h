/*
 * agentsManager.h
 *
 *  Created on: Apr 30, 2016
 *      Author: Tania
 */

#ifndef SRC_AGENTSMANAGER_H_
#define SRC_AGENTSMANAGER_H_

#include "agent.h"
#include "email.h"
#include "list.h"

/**
* This type defines end codes for the methods.
*/
typedef enum {
	AGENT_MANAGER_OUT_OF_MEMORY = 0,
	AGENT_MANAGER_INVALID_PARAMETERS = 1,
	AGENT_MANAGER_ALREADY_EXISTS = 2,
	AGENT_MANAGER_AGENT_NOT_EXISTS = 3,
	AGENT_MANAGER_APARTMENT_NOT_EXISTS = 4,
	AGENT_MANAGER_SERVICE_NOT_EXISTS = 5,
	AGENT_MANAGER_APARTMENT_SERVICE_FULL = 6,
	AGENT_MANAGER_SUCCESS = 7

} AgentsManagerResult;

typedef struct agentsManager_t* AgentsManager;

/**
* Allocates a new agentsManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new agentsManager in case of success.
*/
AgentsManager agentsManagerCreate();

/*
* agentsManagerDestroy: Deallocates an existing manager.
* Clears the element by using the stored free function.
*
* @param manager Target Agents Manager to be deallocated.
* If manager is NULL nothing will be done
*/
void agentsManagerDestroy(AgentsManager manager);

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
		char* company_name, int tax_percentage);

/**
* agentsManagerRemove: removes the given Agent from the collection.
* note that the Agent will not be deallocated!
*
* @param manager Target Agents Manager to remove from.
* @param email   the email of Target Agent to remove.
*
* @return
* 	AGENT_MANAGER_INVALID_PARAMETERS - if Agent is NULL.
* 	AGENT_MANAGER_NOT_EXISTS - if Agent is not registered.
* 	AGENT_MANAGER_SUCCESS - in case of success.
*/
AgentsManagerResult agentsManagerRemove(AgentsManager manager, Email email);

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
		Email email, char* serviceName, int max_apartments) ;

/**
* agentManagerRemoveApartmentService: remove apartment service
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
AgentsManagerResult agentsManagerRemoveApartmentService(AgentsManager manager,
										Email email, char* serviceName);

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
	char* matrix);

/**
* agentManagerAddApartmentToService: add apartment to apartment service
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
AgentsManagerResult agentsManagerRemoveApartmentFromService(
	AgentsManager manager, Email email, char* serviceName, int apartmentId);

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
bool agentsManagerAgentExists(AgentsManager manager, Email email);

/**
* agentFindMatch: finds a matching apartment in each of the agent's services
*
* @param manager   	the agent manager
* @param min_rooms  the minimum amounts of rooms in the requested apartment
* @param min_area   the minimum area in the requested apartment
* @param max_price  the maximum price of the apartment
* @param details out parameter AgentDetails array with the details of the
* 				 agents who has the matching apartments
*
* @return
*	AGENT_MANAGER_APARTMENT_NOT_EXISTS  if the matching apartment is not found
*	AGENT_MANAGER_OUT_OF_MEMORY                 if any of the allocations failed
*	AGENT_MANAER_SUCCESS                at least one match is found
*/
AgentsManagerResult agentManagerFindMatch(AgentsManager manager, int min_rooms,
					 int min_area, int max_price, List* result_list);


#endif /* SRC_AGENTSMANAGER_H_ */
