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
AgentDetails agentDetailsCreate(Email email, char* companyName, double rank );

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
* agentDetailsRankCompare: compares two ranks, and returns the difference
* 							between the first rank and the second
*
* @param first - first agent_details to compare
* @param second - second agent_detail to compare
*
* return:
* double -the difference between the first and the second
*/
double agentDetailsRankCompare( AgentDetails first, AgentDetails second);

/**
* agentDetailsDestroy: Deallocates the details instance.
* Clears the element by using the stored free function.
*
* @param agent_details Target agent_details to be deallocated.
*
* If purchase_bill is NULL nothing will be done
*/
void agentDetailsDestroy( AgentDetails agent_details );

/* agentDetailsGetEmail : gets the email of the agent
*
* returns NULL if a NUll details param recieved
* else the email of this agent details
*/
Email agentDetailsGetEmail( AgentDetails details );

/* agentDetailsGetEmail : gets the companyName of the agent
*
* returns NULL if a NUll details param recieved
* else the companyName of this agent details
*/
char* agentDetailsGetCompanyName( AgentDetails details );

/* agentDetailsGetEmail : gets the rank of the agent
*
* returns NULL if a NUll details param recieved
* else the rank of this agent details
*/
double agentDetailsGetRank( AgentDetails details);


#endif /* SRC_AGENTDETAILS_H_ */
