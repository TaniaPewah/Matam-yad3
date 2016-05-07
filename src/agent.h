/*
 * agent.h
 *
 *  Created on: Apr 30, 2016
 *      Author: Tania
 */

#ifndef SRC_AGENT_H_
#define SRC_AGENT_H_

/**
* This type defines end codes for the methods.
*/
#include "apartment_service.h"
#include "agentDetails.h"
#include "email.h"

#define AT_SIGN '@'
#define RANK_EMPTY -1
#define WALL_CHAR 'w'
#define EMPTY_CHAR 'e'

typedef enum {
	AGENT_OUT_OF_MEMORY = 0,
	AGENT_INVALID_PARAMETERS = 1,
	AGENT_APARTMENT_SERVICE_FULL = 2,
	AGENT_APARTMENT_EXISTS = 3,
	AGENT_APARTMENT_NOT_EXISTS = 4,
	AGENT_APARTMENT_SERVICE_NOT_EXISTS = 6,
	AGENT_SUCCESS = 7
} AgentResult;

typedef struct Agent_t* Agent;

/**
 * agentGetMail: gets the agents mail
 *
 * @param  agent  - Target Agent
 *
 * @return the agent's mail
 */
Email agentGetMail(Agent agent);

/**
* agentGetCompany: gets the agents companyName
*
* @param  agent  - Target Agent
*
* @return the agent's companyName
*/
char* agentGetCompany( Agent agent );

/**
* Allocates a new agent.
*
* Creates a new agent. This function receives the agent email and agent
*  company name and tax percentage
*  and retrieves the new agent created.
*
* @param email   		agents email.
* @param companyName   agents company name
* @param taxPercentge  percentage of tax the agent takes
* @param result 		pointer to save the result agent in
*
* @return
*
* 	AGENT_INVALID_PARAMETERS - if email, companyName or result are NULL or
* 	taxPercentge is not a valid tax percentage.
* 	AGENT_OUT_OF_MEMORY - if allocations failed
* 	AGENT_SUCCESS - in case of success.  A new agent is saved in the result
* 	parameter.
*/
AgentResult agentCreate(Email email, char* companyName, int taxPercentge,
		 Agent* result);

/**
* AgentDestroy: Deallocates an existing agent.
* Clears the element by using the stored free function.
*
* @param agent Target agent to be deallocated.
* If agent is NULL nothing will be done
*/
void agentDestroy(Agent agent);

/**
 * agentGetTax: gets the agents tax percentage
 *
 * @param  agent  - Target Agent
 *
 * @return the agent's  tax percentage
 */
int agentGetTax(Agent agent);

/**
* agentCopy: Allocates a new agent, identical to the old agent
*
* Creates a new agent. This function receives a agent, and retrieves
* a new identical agent pointer in the out pointer parameter.
*
* @param agent the original agent.
* @param result pointer to save the new agent in.
*
* @return
*
* 	AGENT_INVALID_PARAMETERS - if agent or pointer -are NULL.
*
* 	AGENT_OUT_OF_MEMORY - if allocations failed.
*
* 	AGENT_SUCCESS - in case of success. A new agent is saved in the result.
*/
AgentResult agentCopy(Agent agent, Agent* result);

/**
* agentAddApartmentToService: add apartment to apartment service
* 									  of requested agent
* @param agent   	 the requested agent
* @param serviceName a name of the service to add the apartment to
* @param apartment	 the apartment to add
*
* @return
*	AGENT_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_APARTMENT_SERVICE_FULL       if service is full
*	AGENT_APARTMENT_SERVICE_NOT_EXISTS service with the given name doesn't exist
*	AGENT_APARTMENT_EXISTS     if apartment with the given id already exist
*	AGENT_OUT_OF_MEMORY      if allocation failed
*	AGENT_SUCCESS            if apartment successfully added
*/
AgentResult agentAddApartmentToService(Agent agent, char* service_name, int id,
		int price, int width, int height, char* matrix);

/**
* agentRemoveApartmentFromService: remove apartment from apartment service
* 									  of requested agent
* @param agent   	 the requested agent
* @param apartmentId  the apartmentId to remove
* @param serviceName a name of the service to remove the apartment from
*
* @return
*	AGENT_INVALID_PARAMETERS   if any of parameters are NULL
*	AGENT_APARTMENT_NOT_EXISTS if apartment not found in the service
* 	AGENT_APARTMENT_SERVICE_NOT_EXISTS service with the given name doesn't exist
*	AGENT_OUT_OF_MEMORY      if allocation failed
*	AGENT_SUCCESS            if apartment successfully added
*/
AgentResult agentRemoveApartmentFromService(Agent agent, int apartmentId,
	char* serviceName);

/**
* agentGetService: gets the apartment service according to name
*
* @param agent - target agent
* @param serviceName  the name of the requested service
*
* @return
*
* 	NULL  if agent is NULL or the service by this name is not found
*	apartment service otherwise
*/
ApartmentService agentGetService(Agent agent, char* serviceName);

/**
 * agentAddService: adds the apartment service to the requested agent
 *
 * @param agent - target agent
 * @param serviceName  the name of the requested service
 * @param max_apartments the max number of apartments allowed in service
 *
 * @return
 *
 * 	AGENT_INVALID_PARAMETERS  if any of the parameters is NULL
 *
 * 	AGENT_OUT_OF_MEMORY       if failed to add the service to serviceMap
 *
 *	AGENT_SUCCESS    		  if succeeded
 */
AgentResult agentAddService(Agent agent, char* serviceName,
		int max_apartments);

/**
 * agentAddService: removes the apartment service from the requested agent
 *
 * @param agent - target agent
 * @param serviceName  the name of the requested service
 *
 * @return
 *
 * 	AGENT_INVALID_PARAMETERS  			if any of the parameters is NULL
 * 	AGENT_APARTMENT_SERVICE_NOT_EXISTS 	if the service does not exist
 *	AGENT_SUCCESS    		  			if succeeded
 */
AgentResult agentRemoveService(Agent agent, char* service_name);

/**
* agentFindMatch: finds a matching apartment in each of the agent's services
*
* @param agent   	the requested agent
* @param min_rooms  the minimum amounts of rooms in the requested apartment
* @param min_area   the minimum area in the requested apartment
* @param max_price  the maximum price of the apartment
* @param details out parameter AgentDetails instance with the details of the
* 				 agent - if the requested apartment was found in at least one
* 				 of the apartment services that the
*
* @return
* *   AGENT_INVALID_PARAMETERS			if any of the parameters are NULL
*	AGENT_APARTMENT_NOT_EXISTS          if the matching apartment is not found
*	AGENT_APARTMENT_SERVICE_NOT_EXISTS  if the current agent has no apartment
*										services
*	AGENT_OUT_OF_MEMORY                 if any of the allocations failed
*	AGENT_SUCCESS                       a match is found
*/
AgentResult agentFindMatch(Agent agent, int min_rooms, int min_area,
									int max_price, AgentDetails* details);

/**
* agentGetRank: calculates the rank of the agent according to a formula
*
* @param agent 	the requested agent
*
* @return
*	the rank of the agent agent if agent has at least 1 apartment, and -1
*	if has no apartments
*/
double agentGetRank( Agent agent );

/**
* agentGetApartmentDetails: finds the apartment and retrieves its details
*
* @param agent			 the agent
* @param service_name	 the apartment's service name
* @param id				 the apartment's id
* @param apartment_area	 pointer to save the apartment's area
* @param apartment_rooms pointer to save the apartment's room count
* @param apartment_price pointer to save the apartment's price
*
* @return
* 	AGENT_INVALID_PARAMETERS if agent, service_name,
* 		apartment_area, apartment_rooms or apartment_price are NULL.
*
* 	AGENT_SERVICE_DOES_NOT_EXIST if agent has no service under the
* 		given service name.
*
*	AGENT_APARTMENT_NOT_EXISTS if the matching apartment is not found
*
*	AGENT_SUCCESS apartment found.
*/
AgentResult agentGetApartmentDetails(Agent agent, char* service_name,
	int id, int *apartment_area, int *apartment_rooms, int *apartment_price);

#endif /* SRC_AGENT_H_ */
