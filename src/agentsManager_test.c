#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "utilities.h"
#include "agentsManager.h"
#include "agent.h"
#include "list.h"
#include "map.h"
#include "apartment_service.h"

#define PRICE 100
#define AP_ID 1
#define SER_ID 2
#define TAX_PERCENT 5
#define MAX_APARTMENTS 2

static bool testAgentsManagerAddService();
static bool testAgentsManagerRemoveService();
static bool testAgentsManagerAddAgent();
static bool testAgentsManagerRemoveAgent();
static bool testAgentsManagerCreate();
static bool testAgentsManagerAddApartmentToService();
static bool testAgentsManagerRemoveApartmentFromService();
static bool testAgentManagerFindMatch();
static bool testAgentManagerGetSignificantAgents();
static bool testAgentsManagerGetApartmentDetails();
static void freeListElement(ListElement element);
static ListElement copyListElement(ListElement element);

int RunAgentManagerTest() {
	RUN_TEST(testAgentsManagerCreate);
	RUN_TEST(testAgentsManagerAddAgent);
	RUN_TEST(testAgentsManagerRemoveAgent);
	RUN_TEST(testAgentsManagerAddService);
	RUN_TEST(testAgentsManagerRemoveService);
	RUN_TEST(testAgentsManagerAddApartmentToService);
	RUN_TEST(testAgentsManagerRemoveApartmentFromService);
	RUN_TEST(testAgentManagerFindMatch);
	RUN_TEST(testAgentManagerGetSignificantAgents);
	RUN_TEST(testAgentsManagerGetApartmentDetails);
	return 0;
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
	emailCreate("baba@ganosh", &email);
	ASSERT_TEST(email != NULL);
	AgentsManager manager = NULL;
	AgentsManagerResult result = agentsManagerAdd(manager, email,"tania",
			TAX_PERCENT);
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	manager = agentsManagerCreate();
	ASSERT_TEST(manager != NULL);
	result = agentsManagerAdd(manager, email,"tania", TAX_PERCENT);
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	result = agentsManagerAdd(manager, email,"tania",TAX_PERCENT);
	ASSERT_TEST(result == AGENT_MANAGER_ALREADY_EXISTS);
	agentsManagerDestroy(manager);
	return true;
}

static bool testAgentsManagerRemoveAgent(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",TAX_PERCENT);
	Email mail = NULL;
	AgentsManagerResult result = agentsManagerRemove(manager, mail);
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	emailCreate("jjj@ff", &mail);
	result = agentsManagerRemove(manager, mail);
	ASSERT_TEST(result == AGENT_MANAGER_AGENT_NOT_EXISTS);
	result = agentsManagerRemove(manager, email);
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	agentsManagerDestroy(manager);
	return true;
}

static bool testAgentsManagerAddService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	ASSERT_TEST(email != NULL);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",5);
	AgentsManagerResult result =
		agentsManagerAddApartmentService(manager, email, NULL, SER_ID);
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	Email mail = NULL;
	emailCreate("baba@gansh", &mail);
	result = agentsManagerAddApartmentService(manager, mail, "serveMe", SER_ID);
	ASSERT_TEST(result == AGENT_MANAGER_AGENT_NOT_EXISTS);
	result =
			agentsManagerAddApartmentService(manager, email, "serveMe", SER_ID);
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	result =
			agentsManagerAddApartmentService(manager, email, "serveMe", SER_ID);
	ASSERT_TEST(result == AGENT_MANAGER_ALREADY_EXISTS);
	agentsManagerDestroy(manager);
	return true;
}

static bool testAgentsManagerRemoveService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",TAX_PERCENT );
	agentsManagerAddApartmentService(manager, email, "serveMe", SER_ID);
	Email mail = NULL;
	emailCreate("baba@gansh", &mail);
	AgentsManagerResult result =
		agentsManagerRemoveApartmentService(manager, mail, "serveMe");
	ASSERT_TEST(result == AGENT_MANAGER_AGENT_NOT_EXISTS);
	result = agentsManagerRemoveApartmentService(manager, email, "serveMte");
	ASSERT_TEST(result == AGENT_MANAGER_SERVICE_NOT_EXISTS);
	result = agentsManagerRemoveApartmentService(manager, email, "serveMe");
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	agentsManagerDestroy(manager);
	return true;
}

static bool testAgentsManagerAddApartmentToService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",TAX_PERCENT);
	agentsManagerAddApartmentService(manager, email, "serveMe", MAX_APARTMENTS);
	Email mail = NULL;

	AgentsManagerResult result = agentsManagerAddApartmentToService( manager,
								mail, "serveMe", AP_ID ,PRICE, 2, 1, "we");
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	emailCreate("baba@gansh", &mail);

	result = agentsManagerAddApartmentToService(manager, mail, "serveMe",
												AP_ID ,PRICE, 2, 1, "we" );
	ASSERT_TEST(result == AGENT_MANAGER_AGENT_NOT_EXISTS);

	result = agentsManagerAddApartmentToService(manager, email, "sveMe",
												AP_ID ,PRICE, 2, 1, "we" );
	ASSERT_TEST(result == AGENT_MANAGER_SERVICE_NOT_EXISTS);

	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
													AP_ID ,PRICE, 2, 1, "we" );
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);

	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
													AP_ID ,PRICE, 2, 1, "we" );
	ASSERT_TEST(result == AGENT_MANAGER_ALREADY_EXISTS);

	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
												-1 ,PRICE, 2, 1, "we" );
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);

	agentsManagerAddApartmentToService(manager, email, "serveMe",
													2 ,PRICE, 2, 1, "we" );

	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
													3 ,PRICE, 2, 1, "we" );
	ASSERT_TEST(result == AGENT_MANAGER_APARTMENT_SERVICE_FULL);
	agentsManagerDestroy(manager);
	return true;
}

static bool testAgentsManagerRemoveApartmentFromService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",5);
	agentsManagerAddApartmentService(manager, email, "serveMe", 2);
	Email mail = NULL;
	emailCreate("baba@gansh", &mail);
	AgentsManagerResult result = agentsManagerAddApartmentToService
		(manager, email, "serveMe", AP_ID , PRICE, 2, 1, "we");

	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	result = agentsManagerRemoveApartmentFromService(manager, email,"", 0);
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	result = agentsManagerRemoveApartmentFromService(manager, email,
		"serveMee", 1);
	ASSERT_TEST(result == AGENT_MANAGER_SERVICE_NOT_EXISTS);
	result = agentsManagerRemoveApartmentFromService(manager, email,
		"serveMe", 2);
	ASSERT_TEST(result == AGENT_MANAGER_APARTMENT_NOT_EXISTS);
	result = agentsManagerRemoveApartmentFromService(manager, mail,
		"serveMe", 1);
	ASSERT_TEST(result == AGENT_MANAGER_AGENT_NOT_EXISTS);
	result = agentsManagerRemoveApartmentFromService(manager, email,
		"serveMe", 1);
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);

	agentsManagerDestroy(manager);
	emailDestroy(email);
	emailDestroy(mail);
	return true;
}

static bool testAgentManagerFindMatch(){

	Email email = NULL;
	emailCreate( "baba@ganosh", &email );

	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, email, "tania" , TAX_PERCENT);

	agentsManagerAddApartmentService( manager, email, "serveMe", 2 );

	Email mail = NULL;
	emailCreate( "baba@gansh", &mail );
	agentsManagerAdd( manager, mail, "alon" , TAX_PERCENT);

	agentsManagerAddApartmentService( manager, mail, "serveMe", 2 );

	AgentsManagerResult result;
	agentsManagerAddApartmentToService(	manager, email, "serveMe",
				AP_ID,300, 1, 2, "we" );
	agentsManagerAddApartmentToService(	manager, mail, "serveMe",
				2, 200, 2, 2, "weew" );

	List agents_list = listCreate(copyListElement, freeListElement);

	result = agentManagerFindMatch( manager, 1, 1 , 1000, &agents_list);
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );
	ASSERT_TEST( listGetSize(agents_list) == 2);

	result = agentManagerFindMatch( manager, 1, 4 , 1000, &agents_list);
		ASSERT_TEST( result == AGENT_MANAGER_APARTMENT_NOT_EXISTS );

	listClear(agents_list);
	result = agentManagerFindMatch( manager, 1, 2 ,200 ,&agents_list);
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );

	ASSERT_TEST( listGetSize(agents_list) == 1);

	listClear(agents_list);
	result = agentManagerFindMatch( manager, 1, 1 , 100, &agents_list);
	ASSERT_TEST( result == AGENT_MANAGER_APARTMENT_NOT_EXISTS );

	ASSERT_TEST( listGetSize(agents_list) == 0);

	agentsManagerDestroy(manager);
	listDestroy( agents_list);
	emailDestroy(email);
	emailDestroy(mail);
	return true;
}

static bool testAgentManagerGetSignificantAgents(){

	Email email = NULL;
	emailCreate( "baba@ganosh", &email );

	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, email, "tania" , TAX_PERCENT);
	AgentsManagerResult result;
	agentsManagerAddApartmentService( manager, email, "serveMee", 2 );

	Email mail = NULL;
	emailCreate( "baba@gansh", &mail );
	agentsManagerAdd( manager, mail, "alon" , TAX_PERCENT);

	agentsManagerAddApartmentService( manager, mail, "serveMe", 2 );

	agentsManagerAddApartmentToService( manager, email, "serveMee",
				AP_ID, 200, 2, 2,	"weee" );
	agentsManagerAddApartmentToService(	manager, mail, "serveMe",
				AP_ID, 200, 1, 2,	"we" );

	List agents_list = listCreate(copyListElement, freeListElement);

	result = agentManagerGetSignificantAgents( manager, 2, &agents_list );
	ASSERT_TEST( listGetSize( agents_list ) == 2);
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );

	result = agentManagerGetSignificantAgents( manager, 1, &agents_list );
	ASSERT_TEST( listGetSize( agents_list ) == 1);
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );
	ASSERT_TEST( strcmp(agentDetailsGetCompanyName(
			(AgentDetails)(listGetFirst(agents_list))), "tania") );

	agentsManagerDestroy(manager);
	listDestroy( agents_list);
	emailDestroy(email);
	emailDestroy(mail);

	return true;
}

static bool testAgentsManagerGetApartmentDetails(){

	Email email = NULL;
	emailCreate( "baba@ganosh", &email );
	Email mail = NULL;
	emailCreate( "baba@gah", &mail );

	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, email, "tania" , TAX_PERCENT);
	agentsManagerAddApartmentService( manager, email, "serveMe", 2 );
	agentsManagerAddApartmentToService( manager, email, "serveMe",
										AP_ID, 200, 2, 2,	"weee" );

	int area;
	int rooms_num;
	int price;
	int commition;

	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, email, "serveMe",
							AP_ID, &area, &rooms_num, &price, NULL ) ==
									AGENT_MANAGER_INVALID_PARAMETERS);
	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, email, "serveMe",
							AP_ID, &area, &rooms_num, NULL, &commition ) ==
									AGENT_MANAGER_INVALID_PARAMETERS);
	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, email, "serveMe",
							AP_ID, &area, NULL, &price, &commition ) ==
									AGENT_MANAGER_INVALID_PARAMETERS);
	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, email, "serveMe",
							AP_ID, NULL, &rooms_num, &price, &commition ) ==
									AGENT_MANAGER_INVALID_PARAMETERS);
	ASSERT_TEST( agentsManagerGetApartmentDetails( NULL, email, "serveMe",
							AP_ID, &area, &rooms_num, &price, &commition ) ==
									AGENT_MANAGER_INVALID_PARAMETERS);
	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, NULL, "serveMe",
						AP_ID, &area, &rooms_num, &price, &commition ) ==
								AGENT_MANAGER_INVALID_PARAMETERS);
	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, NULL, "serveMe",
							-2, &area, &rooms_num, &price, &commition ) ==
									AGENT_MANAGER_INVALID_PARAMETERS);


	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, mail, "serveMe",
							AP_ID, &area, &rooms_num, &price, &commition ) ==
									AGENT_MANAGER_AGENT_NOT_EXISTS);

	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, email, "ser",
							AP_ID, &area, &rooms_num, &price, &commition ) ==
									AGENT_MANAGER_SERVICE_NOT_EXISTS);

	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, email, "serveMe",
							5, &area, &rooms_num, &price, &commition ) ==
									AGENT_MANAGER_APARTMENT_NOT_EXISTS);

	ASSERT_TEST( agentsManagerGetApartmentDetails( manager, email, "serveMe",
							AP_ID, &area, &rooms_num, &price, &commition ) ==
										AGENT_MANAGER_SUCCESS);
	ASSERT_TEST( area == 3);
	ASSERT_TEST( rooms_num == 1);
	ASSERT_TEST( price == 200);
	ASSERT_TEST( commition == TAX_PERCENT);

	agentsManagerDestroy(manager);
	emailDestroy(email);
	emailDestroy(mail);
	return true;
}

/** Function to be used for freeing data elements from list */
void freeListElement(ListElement element) {
	if (element != NULL)
		agentDetailsDestroy((AgentDetails)element);
}

/** Function to be used for coping data elements from list */
ListElement copyListElement(ListElement element) {
	AgentDetails agent_details =
			agentDetailsCopy((AgentDetails)element);
	return agent_details;
}
