#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "agentsManager.h"
#include "agent.h"
#include "apartment_service.h"

static bool testAgentsManagerAddService();
static bool testAgentCreate();
static bool testAgentsManagerAddAgent();
static bool testAgentsManagerCreate();

//int main() {
int RunAgentManagerTest() {

	RUN_TEST( testAgentsManagerCreate );
	RUN_TEST( testAgentCreate );
	RUN_TEST( testAgentsManagerAddAgent);
	RUN_TEST( testAgentsManagerAddService );
	return 0;
}

static bool testAgentCreate(){

	Email email = NULL;
	emailCreate( "baba@ganosh", &email );
	ASSERT_TEST( email != NULL);

	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);
	ASSERT_TEST(agent != NULL);

	agentDestroy(agent);
	return true;
}

/**
 * Test create & destroy method
 */
static bool testAgentsManagerCreate() {
	AgentsManager manager = agentsManagerCreate();
	ASSERT_TEST(manager != NULL);
	agentsManagerDestroy(manager);
	return true;
}

/**
 * Test create method
 */
static bool testAgentsManagerAddAgent() {

	Email email = NULL;
	emailCreate( "baba@ganosh", &email );
	ASSERT_TEST( email != NULL);
	Agent agent = NULL;
	agentCreate(email,"tania",5, &agent);
	ASSERT_TEST(agent != NULL);

	AgentsManager manager = agentsManagerCreate();
	ASSERT_TEST(manager != NULL);
	AgentsManagerResult result = agentsManagerAdd( manager, agent );
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );
	agentsManagerDestroy(manager);
	agentDestroy(agent);
	return true;
}

static bool testAgentsManagerAddService(){
	Email email = NULL;
	emailCreate( "baba@ganosh", &email );
	ASSERT_TEST( email != NULL);
	Agent agent = NULL;
	agentCreate(email,"tania",5, &agent);
	ASSERT_TEST(agent != NULL);

	AgentsManager manager = agentsManagerCreate();
	ASSERT_TEST(manager != NULL);
	AgentsManagerResult result = agentsManagerAdd( manager, agent );
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );
	ApartmentService service = serviceCreate( 2 );

	result = agentsManagerAddApartmentService( manager, email, service, "serveMe");
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );

	serviceDestroy(service);
	agentsManagerDestroy(manager);
	agentDestroy(agent);
	return true;
}

// check remove service - all cases
// check add agent - all cases
// check remove agent - all cases




