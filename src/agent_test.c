#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "utilities.h"
#include "agent.h"
#include "email.h"
#include "apartment_service.h"

static bool testAgentCreate();
static bool testAgentGetters();
static bool testAgentAddService();
static bool testAgentRemoveService();
static bool testAgentAddApartmentToService();
static bool testAgentRemoveApartmentFromService();
static bool testAgentFindMatch();
static bool testAgentGetApartmentDetails();
static bool testAgentCopy();

int RunAgentTest() {
	RUN_TEST( testAgentCreate );
	RUN_TEST( testAgentGetters);
	RUN_TEST( testAgentAddService);
	RUN_TEST( testAgentRemoveService);
	RUN_TEST( testAgentAddApartmentToService);
	RUN_TEST( testAgentRemoveApartmentFromService);
	RUN_TEST( testAgentFindMatch );
	RUN_TEST( testAgentGetApartmentDetails );
	RUN_TEST( testAgentCopy );
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

	ASSERT_TEST(agentCreate(email,"tania", 5, &agent)
				== AGENT_SUCCESS);

	agentDestroy(agent);
	emailDestroy(email);
	return true;
}

static bool testAgentGetters(){

	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;

	ASSERT_TEST( agentGetCompany( agent ) == NULL );

	agentCreate(email,"tania", 5, &agent);
	ASSERT_TEST( areStringsEqual(  agentGetCompany( agent ), "tania"));

	ASSERT_TEST( emailAreEqual( agentGetMail( agent ), email ));

	ASSERT_TEST( agentGetRank( agent ) == -1 );
	agentAddService(agent,"serveMe", 2);
	agentAddApartmentToService( agent, "serveMe", 1 , 100 , 1, 2, "we" );
	double rank = agentGetRank( agent );
	ASSERT_TEST( rank == 1100100 );

	ASSERT_TEST( agentGetService( agent, "servee" ) == NULL );
	ASSERT_TEST( agentGetService( agent, "serveMe" ) != NULL );

	ASSERT_TEST( agentGetTax( agent ) == 5 );

	agentDestroy(agent);
	emailDestroy(email);
	return true;
}
static bool testAgentAddService(){

	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);

	ASSERT_TEST( agentAddService(agent,"serveMe", -1) ==
									AGENT_INVALID_PARAMETERS);

	ASSERT_TEST( agentAddService(NULL,"serveMe", 1) ==
									AGENT_INVALID_PARAMETERS );

	ASSERT_TEST( agentAddService(agent,"serveMe", 1) ==
										AGENT_SUCCESS );

	ASSERT_TEST( agentAddService(agent,"serve2", 1) ==
											AGENT_SUCCESS );

	agentDestroy(agent);
	emailDestroy(email);
	return true;
}
static bool testAgentRemoveService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);

	agentAddService(agent,"serveMe", 1);

	ASSERT_TEST( agentRemoveService(agent,"e") ==
			AGENT_APARTMENT_SERVICE_NOT_EXISTS);
	ASSERT_TEST( agentRemoveService(NULL,"serveMe") ==
				AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentRemoveService(agent,"serveMe") ==
					AGENT_SUCCESS);

	agentDestroy(agent);
	emailDestroy(email);
	return true;
}
static bool testAgentAddApartmentToService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);

	agentAddService(agent,"serveMe", 2);
	ASSERT_TEST( agentAddApartmentToService(
					NULL, "serveMe", 1 , 100 , 1, 2, "we" ) ==
							AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 1 , 100 , 1, 0, "we" ) ==
							AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 1 , 10 , 1, 2, "we" ) ==
							AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "sere", 1 , 100 , 1, 2, "we" ) ==
							AGENT_APARTMENT_SERVICE_NOT_EXISTS);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 1 , 100 , 1, 2, "we" ) ==
							AGENT_SUCCESS);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 1 , 100 , 2, 2, "weew" ) ==
							AGENT_APARTMENT_EXISTS);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 2 , 100 , 2, 2, "weew" ) ==
							AGENT_SUCCESS);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 3 , 100 , 2, 2, "weew" ) ==
							AGENT_APARTMENT_SERVICE_FULL);
	agentDestroy(agent);
	emailDestroy(email);
	return true;
}
static bool testAgentRemoveApartmentFromService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);

	agentAddService(agent,"serveMe", 2);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 2 , 100 , 2, 2, "weew" ) ==
							AGENT_SUCCESS);

	ASSERT_TEST( agentRemoveApartmentFromService( agent, -1, "serveMe") ==
			AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentRemoveApartmentFromService( NULL, 2, "serveMe") ==
			AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentRemoveApartmentFromService( agent, 2, "se") ==
				AGENT_APARTMENT_SERVICE_NOT_EXISTS);
	ASSERT_TEST( agentRemoveApartmentFromService( agent, 4, "serveMe") ==
				AGENT_APARTMENT_NOT_EXISTS);
	ASSERT_TEST( agentRemoveApartmentFromService( agent, 2, "serveMe") ==
				AGENT_SUCCESS);

	agentDestroy(agent);
	emailDestroy(email);
	return true;
}
static bool testAgentFindMatch(){

	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);
	AgentDetails details = NULL;
	ASSERT_TEST( agentFindMatch(agent, 1,1, 1000, &details ) ==
			AGENT_APARTMENT_SERVICE_NOT_EXISTS);

	agentAddService(agent,"serveMe", 2);
	ASSERT_TEST( agentAddApartmentToService(
					agent, "serveMe", 1 , 200 , 1, 2, "we" ) ==
							AGENT_SUCCESS);

	ASSERT_TEST( agentFindMatch(NULL, 1,1, 1000, &details ) ==
				AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentFindMatch(agent, 1,1, 100, &details ) ==
			AGENT_APARTMENT_NOT_EXISTS);
	ASSERT_TEST( agentFindMatch(agent, 1,3, 300, &details ) ==
			AGENT_APARTMENT_NOT_EXISTS);
	ASSERT_TEST( agentFindMatch(agent, 4,7, 1000, &details ) ==
			AGENT_APARTMENT_NOT_EXISTS);

	ASSERT_TEST( agentAddApartmentToService(
						agent, "serveMe", 2 , 2000 , 2, 2, "wewe" ) ==
								AGENT_SUCCESS);

	ASSERT_TEST( agentFindMatch(agent, 1,1, 1000, &details ) ==
			AGENT_SUCCESS);

	ASSERT_TEST( areStringsEqual( agentDetailsGetCompanyName(details),
									"tania"));

	agentDetailsDestroy(details);
	agentDestroy(agent);
	emailDestroy(email);
	return true;
}
static bool testAgentCopy(){

	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);

	agentAddService(agent,"serveMe", 2);

	Agent copy = NULL;
	ASSERT_TEST( agentCopy(agent, NULL) == AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentCopy(NULL, &copy) == AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentCopy(agent, &copy) == AGENT_SUCCESS);

	ASSERT_TEST( emailAreEqual(agentGetMail(agent) , agentGetMail(copy)));

	agentDestroy(agent);
	agentDestroy(copy);
	emailDestroy(email);
	return true;
}

static bool testAgentGetApartmentDetails(){

	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania", 5, &agent);
	agentAddService(agent,"serveMe", 2);

	int area;
	int rooms_num;
	int price;
	ASSERT_TEST( agentGetApartmentDetails( NULL, "serveMe", 1, &area,
							&rooms_num, &price ) ==  AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentGetApartmentDetails( agent, "serveMe", -3, &area,
							&rooms_num, &price ) ==  AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentGetApartmentDetails( agent, "serveMe", 1, NULL,
							&rooms_num, &price ) ==  AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentGetApartmentDetails( agent, "serveMe", 1, &area,
							NULL, &price ) ==  AGENT_INVALID_PARAMETERS);
	ASSERT_TEST( agentGetApartmentDetails( agent, "serveMe", 1, &area,
							&rooms_num, NULL ) ==  AGENT_INVALID_PARAMETERS);

	ASSERT_TEST( agentGetApartmentDetails( agent, "see", 1, &area,
				&rooms_num, &price ) ==  AGENT_APARTMENT_SERVICE_NOT_EXISTS);

	ASSERT_TEST( agentGetApartmentDetails( agent, "serveMe", 1, &area,
						&rooms_num, &price ) ==  AGENT_APARTMENT_NOT_EXISTS);

	agentAddApartmentToService( agent, "serveMe", 1 , 200 , 2, 2, "weee" );

	ASSERT_TEST( agentGetApartmentDetails( agent, "serveMe", 1, &area,
							&rooms_num, &price ) ==  AGENT_SUCCESS);

	ASSERT_TEST( agentGetApartmentDetails( agent, "serveMe", 3, &area,
						&rooms_num, &price ) ==  AGENT_APARTMENT_NOT_EXISTS);

	ASSERT_TEST( area == 3);
	ASSERT_TEST( rooms_num == 1);
	ASSERT_TEST( price == 200);
	agentDestroy(agent);
	emailDestroy(email);
	return true;
}
