/*
 * agent.c
 *
 *  Created on: Apr 30, 2016
 *      Author: Tania
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "agent.h"
#include "map.h"

struct Agent_t{
	char* email;
	char* companyName;
	int taxPercentge;
	Map apartmentServices;
};

 bool isEmailValid(const char* email);

 /**
 * agentGetMail: gets the agents mail
 *
 * @param  agent  - Target Agent
 *
 * @return the agent's mail
 */
 char* agentGetMail( Agent agent ){
	 return(agent->email);
 }

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
ApartmentService agentGetService( Agent agent, char* serviceName ){

	ApartmentService service = NULL;

	if( serviceName != NULL )
		service = mapGet( agent->apartmentServices,
						(constMapKeyElement)serviceName );
	return service;
}

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
						   char* serviceName ){
	if( agent == NULL || service == NULL || serviceName == NULL)
		return AGENT_INVALID_PARAMETERS;
	if (mapPut(agent->apartmentServices,
			(constMapKeyElement)service,
			(constMapDataElement)serviceName) != MAP_SUCCESS)
		return AGENT_OUT_OF_MEMORY;
	else
		return AGENT_SUCCESS;
}

/**
* isMainValied: checks if the given email adress is Valid.
*
* @param email email to check.
*
* @return
* 	false if email is NULL or does not contian AT_SIGN, else return true
*/
/*static bool isEmailValid(const char* email) {
	return ((email != NULL) && (strchr(email, AT_SIGN) != NULL));
}*/
