/*
 * agentDetails_test.c
 *
 *  Created on: May 8, 2016
 *      Author: Tania
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "utilities.h"
#include "email.h"
#include "agentDetails.h"

static bool testAgentDetailsCreate();
static bool testAgentDetailsCopy();
static bool testAgentDetailsRankCompare();
static bool testAgentDetailsGetters();

int RunAgentDetailsTest(){
	RUN_TEST(testAgentDetailsCreate);
	RUN_TEST(testAgentDetailsCopy);
	RUN_TEST(testAgentDetailsRankCompare);
	RUN_TEST(testAgentDetailsGetters);
	return 0;
}

static bool testAgentDetailsCreate(){
	Email email = NULL;
	emailCreate("tania@gmail", &email);
	ASSERT_TEST(agentDetailsCreate(NULL,"matam", 1) == NULL);
	ASSERT_TEST(agentDetailsCreate(email, NULL, 1) == NULL);
	AgentDetails details = agentDetailsCreate(email,"matam", 1);
	ASSERT_TEST(details != NULL);
	ASSERT_TEST(emailAreEqual(agentDetailsGetEmail(details), email));
	ASSERT_TEST(areStringsEqual(
			agentDetailsGetCompanyName(details),"matam"));
	ASSERT_TEST(agentDetailsGetRank(details) ==  1);
	agentDetailsDestroy(details);
	emailDestroy(email);
	return true;
}
static bool testAgentDetailsCopy(){
	Email email = NULL;
	emailCreate("tania@gmail", &email);
	AgentDetails details = agentDetailsCreate(email,"matam", 1);
	ASSERT_TEST(agentDetailsCopy(NULL) ==  NULL);
	AgentDetails copy = agentDetailsCopy(details);
	ASSERT_TEST(copy != NULL);
	ASSERT_TEST(emailAreEqual(agentDetailsGetEmail(copy), email));
	ASSERT_TEST(areStringsEqual(
				agentDetailsGetCompanyName(copy),"matam"));
	ASSERT_TEST(agentDetailsGetRank(copy) ==  1);
	agentDetailsDestroy(details);
	agentDetailsDestroy(copy);
	emailDestroy(email);
	return true;
}
static bool testAgentDetailsRankCompare(){
	Email email = NULL;
	emailCreate("tania@gmail", &email);
	AgentDetails details1 = agentDetailsCreate(email,"matam", 1);
	AgentDetails details2 = agentDetailsCreate(email,"matam", -1);
	AgentDetails details3 = agentDetailsCreate(email,"matam", 100);
	ASSERT_TEST(agentDetailsRankCompare(details1, details2) == 2);
	ASSERT_TEST(agentDetailsRankCompare(details3, details1) == 99);
	agentDetailsDestroy(details1);
	agentDetailsDestroy(details2);
	agentDetailsDestroy(details3);
	emailDestroy(email);
	return true;
}
static bool testAgentDetailsGetters(){
	Email email = NULL;
	emailCreate("tania@gmail", &email);
	AgentDetails details1 = agentDetailsCreate(email,"matam", 1);

	ASSERT_TEST(emailAreEqual(agentDetailsGetEmail(NULL), NULL));
	ASSERT_TEST(agentDetailsGetCompanyName(NULL) ==  NULL);

	ASSERT_TEST(agentDetailsGetRank(NULL) ==  -1);
	ASSERT_TEST(emailAreEqual(agentDetailsGetEmail(details1), email));
	ASSERT_TEST(areStringsEqual(
					agentDetailsGetCompanyName(details1),"matam"));
	ASSERT_TEST(agentDetailsGetRank(details1) ==  1);

	agentDetailsDestroy(details1);
	emailDestroy(email);
	return true;
}







