/*
 * agentsManager.h
 *
 *  Created on: Apr 30, 2016
 *      Author: Tania
 */

#ifndef SRC_AGENTSMANAGER_H_
#define SRC_AGENTSMANAGER_H_

#include "agent.h"

/**
* This type defines end codes for the methods.
*/
typedef enum {
	AGENT_MANAGRE_OUT_OF_MEMORY = 0,
	AGENT_MANAGRE_INVALID_PARAMETERS = 1,
	AGENT_MANAGRE_ALREADY_EXISTS = 2,
	AGENT_MANAGRE_NOT_EXISTS = 3,
	AGENT_MANAGRE_SUCCESS = 4
} AgentsManagerCode;

typedef struct agentsManager_t *AgentsManager;

/**
* Allocates a new AgentsManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new AgentsManager in case of success.
*/
AgentsManager AgentsManagerCreate();

/**
* AgentsManagerDestroy: Deallocates an existing manager.
* Clears the element by using the stored free function.
*
* @param manager Target Agents Manager to be deallocated.
* If manager is NULL nothing will be done
*/
void AgentsManagerDestroy(AgentsManager manager);

/**
* AgentsManagerAddAgent: adds the new Agent to the collection.
*
* @param manager Target Agents Manager to add to.
* @param Agent Target Agent to add.
*
* @return
* 	Agent_MANAGRE_INVALID_PARAMETERS - if Agent is NULL.
* 	Agent_MANAGRE_ALREADY_EXISTS - if Agent email already registered.
* 	Agent_MANAGRE_SUCCESS - in case of success.
*/
AgentsManagerCode AgentsManagerAdd(AgentsManager manager, Agent Agent);

/**
* AgentsManagerRemove: removes the given Agent from the collection.
* note that the Agent will not be deallocated!
*
* @param manager Target Agents Manager to remove from.
* @param Agent Target Agent to remove.
*
* @return
* 	Agent_MANAGRE_INVALID_PARAMETERS - if Agent is NULL.
* 	Agent_MANAGRE_NOT_EXISTS - if Agent is not registered.
* 	Agent_MANAGRE_SUCCESS - in case of success.
*/
AgentsManagerCode AgentsManagerRemove(AgentsManager manager, Agent Agent);

/**
* AgentsManagerGetAgent: searches for Agent with the specified email address
*
* @param manager Target Agents Manager to search in.
* @param email address to search. searches by string value and not by pointers.
*
* @return
* 	NULL if manager is NULL or Agent email not registered,
* 	else in case of success.
*/
Agent AgentsManagerGetAgent(AgentsManager manager, char* email);

/**
* AgentManagerAddApartmentService: add apartment service to requested agent
*
* @param manager Target Agents Manager
* @param email  email of the requested agent
* @param appartmentService a pointer to a service to add to the agent
*
* @return
*
* 	AGENT_MANAGRE_OUT_OF_MEMORY 	if memory allocation failed
*	AGENT_MANAGRE_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGRE_ALREADY_EXISTS       if service with same name already exists
*	AGENT_MANAGRE_NOT_EXISTS           if agent by this name does not exist
*	AGENT_MANAGRE_SUCCESS              if service successfully added
*/
AgentsManagerCode AgentManagerAddApartmentService(AgentsManager manager,
										char* email, void* appartmentService);

/**
* AgentManagerRemoveApartmentService: remove apartment service
* 									  from requested agent
* @param manager 	 Target Agents Manager
* @param email   	 email of the requested agent
* @param serviceName a name of the service to remove
*
* @return
*	AGENT_MANAGRE_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_MANAGRE_NOT_EXISTS           if agent by this name does not exist or
*									service with the given name does not exist
*	AGENT_MANAGRE_SUCCESS              if service successfully removed
*/
AgentsManagerCode AgentManagerRemoveApartmentService(AgentsManager manager,
										char* email, char* serviceName);

#endif /* SRC_AGENTSMANAGER_H_ */
