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

bool isEmailValid( char* email);
static bool isTaxValid( int taxPercentage );
char* strdup(const char *str);

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
 * agentGetTax: gets the agents tax percentage
 *
 * @param  agent  - Target Agent
 *
 * @return the agent's  tax percentage
 */
int agentGetTax( Agent agent ){
	return( agent->taxPercentge );
}

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
		 int taxPercentge, Agent* result) {
 	if ((result == NULL) || (!isEmailValid(email)) || !isTaxValid(taxPercentge))
 		return AGENT_INVALID_PARAMETERS;
 	Agent agent = malloc (sizeof(*agent));
 	if (agent == NULL)
 		return AGENT_OUT_OF_MEMORY;
 	agent->email = strdup(email);
 	agent->companyName = companyName ? strdup(companyName) : NULL;
 	if (agent->email == NULL || agent->companyName == NULL ) {
 		free(agent);
 		return AGENT_OUT_OF_MEMORY;
 	} else {
 		agent->taxPercentge = taxPercentge;
 		*result = agent;
 		return AGENT_SUCCESS;
 	}
 }

 /**
 * AgentDestroy: Deallocates an existing agent.
 * Clears the element by using the stored free function.
 *
 * @param agent Target agent to be deallocated.
 * If agent is NULL nothing will be done
 */
 void agentDestroy(Agent agent) {
 	if (agent != NULL) {
 		free(agent->email);
 		free(agent);
 	}
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
AgentResult agentAddService( Agent agent, ApartmentService service,
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
*	AGENT_ALREADY_EXISTS     if apartment with the given id already exist
*	AGENT_OUT_OF_MEMORY      if allocation failed
*	AGENT_SUCCESS            if apartment successfully added
*/
AgentResult agentAddApartmentToService( Agent agent, Apartment apartment,
										 int id, char* serviceName){

	ApartmentService service = agentGetService( agent, serviceName );
	if ( service == NULL )
		return AGENT_APARTMENT_SERVICE_NOT_EXISTS;

	ApartmentServiceResult result =
								serviceAddApartment( service, apartment, id );

	switch (result){
		case APARTMENT_SERVICE_OUT_OF_MEM : {
			return AGENT_OUT_OF_MEMORY;
			break;
		}
		case APARTMENT_SERVICE_ALREADY_EXISTS:{
			return AGENT_APARTMENT_EXISTS;
			break;
		}
		case APARTMENT_SERVICE_FULL:{
			return AGENT_APARTMENT_SERVICE_FULL;
			break;
		}
		case APARTMENT_SERVICE_NULL_ARG:{
			return AGENT_INVALID_PARAMETERS;
			break;
		}
		case APARTMENT_SUCCESS:{
			return AGENT_SUCCESS;
			break;
		}
		default:
			break;
	}

	return AGENT_SUCCESS;
}

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
											char* serviceName ){
	ApartmentService service = agentGetService( agent, serviceName );
		if ( service == NULL )
			return AGENT_APARTMENT_SERVICE_NOT_EXISTS;

		Apartment apartment = NULL;
		ApartmentResult getResult = serviceGetById( service, apartmentId,
													apartment );
		if( getResult == APARTMENT_SERVICE_NULL_ARG )
			return AGENT_INVALID_PARAMETERS;

		if( getResult != APARTMENT_SERVICE_SUCCESS )
			return AGENT_APARTMENT_NOT_EXISTS;

		ApartmentServiceResult deleteResult =
								serviceDeleteApartment( service, apartment );

		switch ( deleteResult ){
			case APARTMENT_SERVICE_EMPTY:{
				return AGENT_APARTMENT_NOT_EXISTS;
				break;
			}
			case APARTMENT_SERVICE_NULL_ARG:{
				return AGENT_INVALID_PARAMETERS;
				break;
			}
			case APARTMENT_SUCCESS:{
				return AGENT_SUCCESS;
				break;
			}
			default:
				break;
		}

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
bool isEmailValid( char* email) {
	return ((email != NULL) && (strchr(email, AT_SIGN) != NULL));
}

static bool isTaxValid( int taxPercentage ){

	return ((taxPercentage >=1) && (taxPercentage <= 100));
}

// TODO take care of strdup
char* strdup(const char* str){

	char *dupstr = malloc(strlen(str) * sizeof(char) + 1);
	strcpy(dupstr, str);

	return dupstr;
}

