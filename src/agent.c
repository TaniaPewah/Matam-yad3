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
#include "utilities.h"
#include "map.h"

struct Agent_t{
	Email email;
	char* companyName;
	int taxPercentge;
	Map apartmentServices;
};

static bool isTaxValid( int taxPercentage );
static MapDataElement GetDataCopy(constMapDataElement data);
static MapKeyElement GetKeyCopy(constMapKeyElement key);
static void FreeData(MapDataElement data);
static void FreeKey(MapKeyElement key);
static int CompareKeys(constMapKeyElement first, constMapKeyElement second);

 /**
 * agentGetMail: gets the agents mail
 *
 * @param  agent  - Target Agent
 *
 * @return the agent's mail
 */
Email agentGetMail( Agent agent ){
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
 AgentResult agentCreate( Email email, char* companyName,
		 int taxPercentge, Agent* result) {
 	if ( result == NULL || email == NULL || !isTaxValid(taxPercentge))
 		return AGENT_INVALID_PARAMETERS;
 	Agent agent = malloc (sizeof(*agent));
 	if (agent == NULL)
 		return AGENT_OUT_OF_MEMORY;
 	EmailResult eResult = emailCopy( email, &(agent->email));

 	if( eResult == EMAIL_OUT_OF_MEMORY ){
 		free(agent);
 		return AGENT_OUT_OF_MEMORY;
 	}
 	agent->companyName = companyName ? duplicateString(companyName) : NULL;
 	if ( agent->companyName == NULL ) {
 		emailDestroy(agent->email);
 		free(agent);
 		return AGENT_OUT_OF_MEMORY;
 	}
 	agent->apartmentServices = mapCreate(GetDataCopy, GetKeyCopy,
 			FreeData, FreeKey, CompareKeys);
	agent->taxPercentge = taxPercentge;
	*result = agent;
	return AGENT_SUCCESS;
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
 		emailDestroy( agent->email );
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
						serviceName );
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
	if (mapPut( agent->apartmentServices, (constMapKeyElement)serviceName,
			(constMapDataElement)service) != MAP_SUCCESS)
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
		ApartmentServiceResult getResult = serviceGetById( service,
												 apartmentId, &apartment );
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

static bool isTaxValid( int taxPercentage ){

	return ((taxPercentage >=1) && (taxPercentage <= 100));
}

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
* 	EMAIL_NULL_PARAMETERS - if agent or pointer -are NULL.
*
* 	EMAIL_OUT_OF_MEMORY - if allocations failed.
*
* 	EMAIL_SUCCESS - in case of success. A new agent is saved in the result.
*/
AgentResult agentCopy(Agent agent, Agent* result_agent){
	if (agent == NULL || result_agent == NULL) return AGENT_INVALID_PARAMETERS;
	Agent new_agent = NULL;
	AgentResult result_state = agentCreate( agent->email,
				agent->companyName, agent->taxPercentge, &new_agent);
	if (result_state != AGENT_SUCCESS) return  AGENT_OUT_OF_MEMORY;
	*result_agent = new_agent;
	return AGENT_SUCCESS;
}


// TODO take care of strdup
char* strdup(const char* str){

	char *dupstr = malloc(strlen(str) * sizeof(char) + 1);
	strcpy(dupstr, str);

	return dupstr;
}

/** Function to be used for copying data elements into the map */
static MapDataElement GetDataCopy(constMapDataElement data) {
	ApartmentService new_service = NULL;
	new_service = serviceCopy( (ApartmentService)data );
	return (MapDataElement)new_service;
}

/** Function to be used for copying key elements into the map */
static MapKeyElement GetKeyCopy(constMapKeyElement key) {
	char* name = NULL;
	name = duplicateString( key );
	return name;
}

/** Function to be used for freeing data elements into the map */
static void FreeData(MapDataElement data) {
	if (data != NULL) serviceDestroy((ApartmentService)data);
}

/** Function to be used for freeing key elements into the map */
static void FreeKey(MapKeyElement key) {
	if (key != NULL) free(key);
}

/** Function to be used for comparing key elements in the map */
static int CompareKeys(constMapKeyElement first, constMapKeyElement second) {
	return strcmp( first, second);
}

