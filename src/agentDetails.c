/*
 * agentDetails.c
 *
 *  Created on: May 6, 2016
 *      Author: Tania
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "agentDetails.h"
#include "utilities.h"
#include "email.h"

struct agentDetails_t {
	Email email;
	char* companyName;
};

/**
* agentDetailsCreate: creates a new AgentDetails instance.
*
* Creates a new AgentDetails. This function receives the client email,
* and the total money the client paid for apartments and retrieves a new
* AgentDetails.
*
* @param email agents email.
* @param companyName the name of the agents company
*
* @return
* 	A new AgentDetails element or NULL if :email is NULL, companyName is null,
* 	or if allocation failed.
*/
AgentDetails agentDetailsCreate(Email email, char* company_name ){

	if (email == NULL || company_name == NULL)
		return NULL;
	AgentDetails agent_details = malloc(sizeof(*agent_details));
	if (agent_details == NULL)
		return NULL;
	agent_details->email = NULL;
	emailCopy(email, &agent_details->email);
	agent_details->companyName = duplicateString( company_name );
	if (agent_details->email == NULL || agent_details->companyName == NULL ) {
		free(agent_details);
		return NULL;
	}
	return agent_details;
}

/**
* agentDetailsCopy: Allocates a new AgentDetails instance,
* identical to the old AgentDetails instance
*
* Creates a new AgentDetails. This function receives a AgentDetails,
* and retrieves a new identical AgentDetails.
*
* @param agent_details the AgentDetails.
*
* @return
* NULL if agent_details is NULL or allocation failed; else returns the new
* AgentDetails.
*/
AgentDetails agentDetailsCopy( AgentDetails agent_details){
	if (agent_details == NULL) return NULL;
		return agentDetailsCreate( agent_details->email,
								   agent_details->companyName);
}

/**
* agentDetailsDestroy: Deallocates the details instance.
* Clears the element by using the stored free function.
*
* @param agent_details Target agent_details to be deallocated.
*
* If purchase_bill is NULL nothing will be done
*/
void agentDetailsDestroy( AgentDetails agent_details ){
	if (agent_details != NULL) {
			emailDestroy(agent_details->email);
			free(agent_details->companyName);
			free(agent_details);
		}
}

