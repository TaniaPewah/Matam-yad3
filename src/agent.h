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
} AgentCode;

typedef struct Agent_t* Agent;

/**
* agentGetMail: gets the agents mail
*
* @param  agent  - Target Agent
*
* @return the agent's mail
*/
char* agentGetMail( Agent agent );

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
AgentCode agentAddService( Agent agent, ApartmentService service,
						   char* serviceName );

#endif /* SRC_AGENT_H_ */
