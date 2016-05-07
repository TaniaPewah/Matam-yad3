#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "agent.h"
#include "apartment_service.h"

static bool testAgentCreate();

//int main() {
int RunAgentTest() {
	RUN_TEST( testAgentCreate );
	return 0;
}

static bool testAgentCreate(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	ASSERT_TEST(email != NULL);
	Agent agent = NULL;
	ASSERT_TEST(agentCreate(NULL,"tania", 5, &agent)
			== AGENT_INVALID_PARAMETERS);
	ASSERT_TEST(agentCreate(email, NULL, 5, &agent)
			== AGENT_INVALID_PARAMETERS);
	ASSERT_TEST(agentCreate(email, "tania", -5, &agent)
			== AGENT_INVALID_PARAMETERS);
	ASSERT_TEST(agentCreate(email, "tania", 0, &agent)
			== AGENT_INVALID_PARAMETERS);
	ASSERT_TEST(agentCreate(email, "tania", 101, &agent)
			== AGENT_INVALID_PARAMETERS);
	ASSERT_TEST(agentCreate(email, "tania", 20, NULL)
			== AGENT_INVALID_PARAMETERS);
	agentCreate(email,"tania", 5, &agent);
	ASSERT_TEST(agent != NULL);
	agentDestroy(agent);
	return true;
}



