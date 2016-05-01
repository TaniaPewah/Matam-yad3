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




#endif /* SRC_AGENT_H_ */
