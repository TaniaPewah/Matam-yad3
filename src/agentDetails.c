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

#define NO_RANK -1

struct agentDetails_t {
	Email email;
	char* companyName;
	double rank;
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
AgentDetails agentDetailsCreate(Email email, char* company_name, double rank) {
	if (email == NULL || company_name == NULL)
		return NULL;
	AgentDetails agent_details = malloc(sizeof(*agent_details));
	if (agent_details == NULL)
		return NULL;
	agent_details->rank = rank;
	agent_details->email = NULL;
	if (emailCopy(email, &agent_details->email) == EMAIL_OUT_OF_MEMORY) {
		free(agent_details);
		return NULL;
	}
	agent_details->companyName = duplicateString(company_name);
	if (agent_details->companyName == NULL) {
		emailDestroy(agent_details->email);
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
AgentDetails agentDetailsCopy(AgentDetails agent_details) {
	if (agent_details == NULL) return NULL;
		return agentDetailsCreate(agent_details->email,
								  agent_details->companyName,
								  agent_details->rank);
}

/**
* agentDetailsDestroy: Deallocates the details instance.
* Clears the element by using the stored free function.
*
* @param agent_details Target agent_details to be deallocated.
*
* If purchase_bill is NULL nothing will be done
*/
void agentDetailsDestroy(AgentDetails agent_details) {
	if (agent_details != NULL) {
		emailDestroy(agent_details->email);
		free(agent_details->companyName);
		free(agent_details);
	}
}

/**
* agentDetailsRankCompare: compares two ranks, and returns the difference
* 							between the first rank and the second
*
* @param first - first agent_details to compare
* @param second - second agent_detail to compare
*
* return:
* int -the difference between the first and the second
*/
double agentDetailsRankCompare(AgentDetails first, AgentDetails second) {

	return (first->rank - second->rank);
}


/* agentDetailsGetEmail : gets the email of the agent
*
* returns NULL if a NUll details param recieved
* else the email of this agent details
*/
Email agentDetailsGetEmail(AgentDetails details) {
	return details != NULL ? details->email : NULL;
}

/* agentDetailsGetEmail : gets the companyName of the agent
*
* returns NULL if a NUll details param recieved
* else the companyName of this agent details
*/
char* agentDetailsGetCompanyName(AgentDetails details) {
	return details != NULL ? details->companyName : NULL;
}

/* agentDetailsGetEmail : gets the rank of the agent
*
* returns NO_RANK if NULL details recieved
* else the rank of this agent details
*/
double agentDetailsGetRank(AgentDetails details) {
	return details != NULL ? details->rank : NO_RANK;
}
