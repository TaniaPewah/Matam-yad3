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
	AGENT_SUCCESS = 2
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

AgentResult agentAddApartmentToService( Agent agent, Apartment apartment,
										char* serviceName );

AgentResult agentRemoveApartmentFromService( Agent agent, Apartment apartment,
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
