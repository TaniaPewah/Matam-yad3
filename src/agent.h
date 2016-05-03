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


#define AT_SIGN '@'

typedef enum {
	AGENT_OUT_OF_MEMORY = 0,
	AGENT_INVALID_PARAMETERS = 1,
	AGENT_APARTMENT_SERVICE_FULL = 2,
	AGENT_APARTMENT_EXISTS = 3,
	AGENT_APARTMENT_NOT_EXISTS = 4,
	AGENT_APARTMENT_SERVICE_NOT_EXISTS = 5,
	AGENT_SUCCESS = 6
} AgentResult;

typedef struct Agent_t* Agent;

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
* 	AGENT_INVALID_PARAMETERS - if one email is NULL or
* 	email does not contain the character AT_SIGN, or result is NULL.
* 	AGENT_OUT_OF_MEMORY - if allocations failed
* 	AGENT_SUCCESS - in case of success.  A new agent is saved in the result
* 	parameter.
*/
AgentResult agentCreate( char* email, char* companyName,
		 int taxPercentge, Agent* result);

/**
* AgentDestroy: Deallocates an existing agent.
* Clears the element by using the stored free function.
*
* @param agent Target agent to be deallocated.
* If agent is NULL nothing will be done
*/
void agentDestroy(Agent agent);

/**
* agentGetMail: gets the agents mail
*
* @param  agent  - Target Agent
*
* @return the agent's mail
*/
char* agentGetMail( Agent agent );

/**
 * agentGetTax: gets the agents tax percentage
 *
 * @param  agent  - Target Agent
 *
 * @return the agent's  tax percentage
 */
int agentGetTax( Agent agent );

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
*	AGENT_ALREADY_EXISTS   if apartment with the given id already exist
*	AGENT_OUT_OF_MEMORY      if allocation failed
*	AGENT_SUCCESS            if apartment successfully added
*/
AgentResult agentAddApartmentToService( Agent agent, Apartment apartment,
									int id, char* serviceName );

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
AgentResult agentRemoveApartmentFromService( Agent agent, int apartmentId,
											char* serviceName );

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
ApartmentService agentGetService( Agent agent, char* serviceName );

/**
 * agentAddService: adds the apartment service to the requested agent
 *
 * @param agent - target agent
 * @param service - the service to add
 * @param serviceName  the name of the requested service
 *
 * @return
 *
 * 	AGENT_INVALID_PARAMETERS  if any of the parameters is NULL
 * 	AGENT_OUT_OF_MEMORY       if failed to add the service to serviceMap
 *	AGENT_SUCCESS    		  if succeeded
 */
AgentResult agentAddService( Agent agent, ApartmentService service,
						   char* serviceName );

#endif /* SRC_AGENT_H_ */
