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

static bool testAgentsManagerAddService();
static bool testAgentsManagerRemoveService();
static bool testAgentsManagerAddAgent();
static bool testAgentsManagerRemoveAgent();
static bool testAgentsManagerCreate();
static bool testAgentsManagerAddApartmentToService();
static bool testAgentsManagerRemoveApartmentFromService();
static bool testAgentManagerFindMatch();
static void freeListElement(ListElement element);
static ListElement copyListElement(ListElement element);

static MapDataElement GetDataCopy(constMapDataElement data);
static MapKeyElement GetKeyCopy(constMapKeyElement key);
static void FreeData(MapDataElement data);
static void FreeKey(MapKeyElement key);
static int CompareKeys(constMapKeyElement first, constMapKeyElement second);



//int main() {
int RunAgentManagerTest() {
	RUN_TEST(testAgentsManagerCreate);
	RUN_TEST(testAgentsManagerAddAgent);
	RUN_TEST(testAgentsManagerRemoveAgent);
	RUN_TEST(testAgentsManagerAddService);
	RUN_TEST(testAgentsManagerRemoveService);
	//RUN_TEST(testAgentsManagerAddApartmentToService);
	//RUN_TEST(testAgentsManagerRemoveApartmentFromService);
	RUN_TEST(testAgentManagerFindMatch);
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
	AgentsManagerResult result = agentsManagerAdd(manager, email,"tania",5);
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	manager = agentsManagerCreate();
	ASSERT_TEST(manager != NULL);
	result = agentsManagerAdd(manager, email,"tania",5);
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	result = agentsManagerAdd(manager, email,"tania",5);
	ASSERT_TEST(result == AGENT_MANAGER_ALREADY_EXISTS);
	agentsManagerDestroy(manager);
	return true;
}

static bool testAgentsManagerRemoveAgent(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",5);
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
		agentsManagerAddApartmentService(manager, email, NULL, 2);
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	Email mail = NULL;
	emailCreate("baba@gansh", &mail);
	result = agentsManagerAddApartmentService(manager, mail, "serveMe", 2);
	ASSERT_TEST(result == AGENT_MANAGER_AGENT_NOT_EXISTS);
	result = agentsManagerAddApartmentService(manager, email, "serveMe", 2);
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	result = agentsManagerAddApartmentService(manager, email, "serveMe", 2);
	ASSERT_TEST(result == AGENT_MANAGER_ALREADY_EXISTS);
	agentsManagerDestroy(manager);
	return true;
}

static bool testAgentsManagerRemoveService(){
	Email email = NULL;
	emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",5);
	agentsManagerAddApartmentService(manager, email, "serveMe", 2);
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
	/*emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",5);
	agentsManagerAddApartmentService(manager, email, "serveMe", 2);
	Email mail = NULL;
	emailCreate("baba@gansh", &mail);
	Apartment apartment = NULL;
	AgentsManagerResult result = agentsManagerAddApartmentToService(manager,
			email, "serveMe", apartment, 1);
	ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	apartment = apartmentCreate(&squarePtr, 1, 2, 100);
	result = agentsManagerAddApartmentToService(manager, mail, "serveMe",
		apartment, 1);
	ASSERT_TEST(result == AGENT_MANAGER_AGENT_NOT_EXISTS);
	result = agentsManagerAddApartmentToService(manager, email, "servieMe",
		apartment, 1);
	ASSERT_TEST(result == AGENT_MANAGER_SERVICE_NOT_EXISTS);
	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
		apartment, 1);
	ASSERT_TEST(result == AGENT_MANAGER_SUCCESS);
	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
		apartment, 1);
	ASSERT_TEST(result == AGENT_MANAGER_ALREADY_EXISTS);
	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
		apartment, -1);
		ASSERT_TEST(result == AGENT_MANAGER_INVALID_PARAMETERS);
	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
		apartment, 2);
	result = agentsManagerAddApartmentToService(manager, email, "serveMe",
		apartment, 3);
	ASSERT_TEST(result == AGENT_MANAGER_APARTMENT_SERVICE_FULL);
	agentsManagerDestroy(manager);*/
	return true;
}

static bool testAgentsManagerRemoveApartmentFromService(){
	Email email = NULL;
	/*emailCreate("baba@ganosh", &email);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd(manager, email,"tania",5);
	agentsManagerAddApartmentService(manager, email, "serveMe", 2);
	Email mail = NULL;
	emailCreate("baba@gansh", &mail);
	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);
	AgentsManagerResult result = agentsManagerAddApartmentToService
		(manager, email, "serveMe", apartment, 1);
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
	agentsManagerDestroy(manager);*/
	return true;
}


static bool testAgentManagerFindMatch(){

	Email email = NULL;/*
	emailCreate( "baba@ganosh", &email );

	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, email, "tania" , 5);

	agentsManagerAddApartmentService( manager, email, "serveMe", 2);

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);

	AgentsManagerResult result;
	agentsManagerAddApartmentToService(	manager, email, "serveMe", apartment, 1 );

	List agents_list = listCreate(copyListElement, freeListElement);
	result = agentManagerFindMatch( manager, 1, 1 , 1000, &agents_list);
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );
	ASSERT_TEST( listGetSize(agents_list) == 1);

	listClear(agents_list);
	result = agentManagerFindMatch( manager, 1, 1 , 10, &agents_list);
	ASSERT_TEST( result == AGENT_MANAGER_APARTMENT_NOT_EXISTS );*/
	return true;
}


/** Function to be used for copying data elements into the map */
static MapDataElement GetDataCopy(constMapDataElement data) {
	ApartmentService new_service = NULL;
	new_service = serviceCopy( (ApartmentService)data );
	return (MapDataElement)new_service;
}

/** Function to be used for copying key elements into the map */
static MapKeyElement GetKeyCopy(constMapKeyElement key) {
	char* name = NULL;
	name = duplicateString( key );
	return name;
}

/** Function to be used for freeing data elements into the map */
static void FreeData(MapDataElement data) {
	if (data != NULL) serviceDestroy((ApartmentService)data);
}

/** Function to be used for freeing key elements into the map */
static void FreeKey(MapKeyElement key) {
	if (key != NULL) free(key);
}

/** Function to be used for comparing key elements in the map */
static int CompareKeys(constMapKeyElement first, constMapKeyElement second) {
	return strcmp( first, second);
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
