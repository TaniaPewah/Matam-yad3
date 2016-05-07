/*
 * agentDetails.h
 *
 *  Created on: May 6, 2016
 *      Author: Tania
 */

#ifndef SRC_AGENTDETAILS_H_
#define SRC_AGENTDETAILS_H_

#include "email.h"

typedef struct agentDetails_t *AgentDetails;

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
AgentDetails agentDetailsCreate(Email email, char* companyName, int rank );

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
AgentDetails agentDetailsCopy( AgentDetails agent_details);

/**
* agentDetailsDestroy: Deallocates the details instance.
* Clears the element by using the stored free function.
*
* @param agent_details Target agent_details to be deallocated.
*
* If purchase_bill is NULL nothing will be done
*/
void agentDetailsDestroy( AgentDetails agent_details );


#endif /* SRC_AGENTDETAILS_H_ */
