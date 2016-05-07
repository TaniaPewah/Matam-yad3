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
static bool testAgentsManagerRemoveService();
static bool testAgentsManagerAddAgent();
static bool testAgentsManagerRemoveAgent();
static bool testAgentsManagerCreate();
static bool testAgentsManagerAddApartmentToService();
static bool testAgentsManagerRemoveApartmentFromService();

//int main() {
int RunAgentManagerTest() {
	RUN_TEST(testAgentsManagerCreate);
	RUN_TEST(testAgentsManagerAddAgent);
	RUN_TEST(testAgentsManagerRemoveAgent);
	RUN_TEST(testAgentsManagerAddService);
	RUN_TEST(testAgentsManagerRemoveService);
	RUN_TEST(testAgentsManagerAddApartmentToService);
	RUN_TEST(testAgentsManagerRemoveApartmentFromService);
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
	emailCreate( "baba@ganosh", &email );
	ASSERT_TEST( email != NULL);
	Agent agent = NULL;
	agentCreate(email,"tania",5, &agent);
	ASSERT_TEST(agent != NULL);
	AgentsManager manager = NULL;

	AgentsManagerResult result = agentsManagerAdd( manager, agent );
	ASSERT_TEST( result == AGENT_MANAGER_INVALID_PARAMETERS );

	manager = agentsManagerCreate();
	ASSERT_TEST(manager != NULL);
	result = agentsManagerAdd( manager, agent );
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS );

	result = agentsManagerAdd( manager, agent );
	ASSERT_TEST( result == AGENT_MANAGER_ALREADY_EXISTS );

	agentsManagerDestroy(manager);
	agentDestroy(agent);
	return true;
}

static bool testAgentsManagerRemoveAgent(){

	Email email = NULL;
	emailCreate( "baba@ganosh", &email );
	Agent agent = NULL;
	agentCreate(email,"tania",5, &agent);
	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, agent );

	Email mail = NULL;
	AgentsManagerResult result = agentsManagerRemove( manager, mail);
	ASSERT_TEST( result == AGENT_MANAGER_INVALID_PARAMETERS );

	emailCreate( "jjj@ff", &mail );

	result = agentsManagerRemove( manager, mail);
	ASSERT_TEST( result == AGENT_MANAGER_AGENT_NOT_EXISTS );

	result = agentsManagerRemove( manager, email);
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
	agentsManagerAdd( manager, agent );
	ApartmentService service = NULL;

	AgentsManagerResult result =
		agentsManagerAddApartmentService( manager, email, service, "serveMe");
	ASSERT_TEST( result == AGENT_MANAGER_INVALID_PARAMETERS );


	service = serviceCreate( 2 );
	Email mail = NULL;
	emailCreate( "baba@gansh", &mail );

	result =
		agentsManagerAddApartmentService( manager, mail, service, "serveMe");
	ASSERT_TEST( result == AGENT_MANAGER_AGENT_NOT_EXISTS );

	result =
		agentsManagerAddApartmentService( manager, email, service, "serveMe");
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS ) ;


	result =
		agentsManagerAddApartmentService( manager, email, service, "serveMe");
		ASSERT_TEST( result == AGENT_MANAGER_ALREADY_EXISTS ) ;


	serviceDestroy(service);
	agentsManagerDestroy(manager);
	agentDestroy(agent);
	return true;
}

static bool testAgentsManagerRemoveService(){
	Email email = NULL;
	emailCreate( "baba@ganosh", &email);
	Agent agent = NULL;
	agentCreate(email,"tania",5, &agent);

	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, agent );
	ApartmentService service = serviceCreate( 2 );
	agentsManagerAddApartmentService( manager, email, service, "serveMe");


	Email mail = NULL;
	emailCreate( "baba@gansh", &mail );
	AgentsManagerResult result =
		agentsManagerRemoveApartmentService( manager, mail, "serveMe");
	ASSERT_TEST( result == AGENT_MANAGER_AGENT_NOT_EXISTS ) ;

	result = agentsManagerRemoveApartmentService( manager, email, "serveMte");
	ASSERT_TEST( result == AGENT_MANAGER_SERVICE_NOT_EXISTS ) ;

	result = agentsManagerRemoveApartmentService( manager, email, "serveMe");
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS ) ;

	serviceDestroy(service);
	agentsManagerDestroy(manager);
	agentDestroy(agent);
	return true;
}

static bool testAgentsManagerAddApartmentToService(){
	Email email = NULL;
	emailCreate( "baba@ganosh", &email );
	Agent agent = NULL;
	agentCreate(email,"tania",5, &agent);
	ASSERT_TEST(agent != NULL);

	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, agent );
	ApartmentService service = serviceCreate( 2 );

	agentsManagerAddApartmentService( manager, email, service, "serveMe");

	Email mail = NULL;
	emailCreate( "baba@gansh", &mail );

	Apartment apartment = NULL;

	AgentsManagerResult result = agentsManagerAddApartmentToService( manager,
			email, "serveMe", apartment, 1 );
	ASSERT_TEST( result == AGENT_MANAGER_INVALID_PARAMETERS ) ;

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;

	apartment = apartmentCreate(&squarePtr, 1, 2, 100);
	result = agentsManagerAddApartmentToService( manager,
										mail, "serveMe", apartment, 1 );
	ASSERT_TEST( result == AGENT_MANAGER_AGENT_NOT_EXISTS ) ;


	result = agentsManagerAddApartmentToService( manager,
											email, "servieMe", apartment, 1 );
	ASSERT_TEST( result == AGENT_MANAGER_SERVICE_NOT_EXISTS ) ;

	result = agentsManagerAddApartmentToService( manager,
											email, "serveMe", apartment, 1 );
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS ) ;

	result = agentsManagerAddApartmentToService( manager,
											email, "serveMe", apartment, 1 );
	ASSERT_TEST( result == AGENT_MANAGER_ALREADY_EXISTS ) ;

	result = agentsManagerAddApartmentToService( manager,
												email, "serveMe", apartment, -1 );
		ASSERT_TEST( result == AGENT_MANAGER_INVALID_PARAMETERS ) ;

	result = agentsManagerAddApartmentToService( manager,
											email, "serveMe", apartment, 2 );
	result = agentsManagerAddApartmentToService( manager,
												email, "serveMe", apartment, 3 );
	ASSERT_TEST( result == AGENT_MANAGER_APARTMENT_SERVICE_FULL ) ;


	serviceDestroy(service);
	agentsManagerDestroy(manager);
	agentDestroy(agent);
	return true;
}

static bool testAgentsManagerRemoveApartmentFromService(){
	Email email = NULL;
	emailCreate( "baba@ganosh", &email );
	Agent agent = NULL;
	agentCreate(email,"tania",5, &agent);
	ASSERT_TEST(agent != NULL);

	AgentsManager manager = agentsManagerCreate();
	agentsManagerAdd( manager, agent );
	ApartmentService service = serviceCreate( 2 );

	agentsManagerAddApartmentService( manager, email, service, "serveMe");

	Email mail = NULL;
	emailCreate( "baba@gansh", &mail );
	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);

	AgentsManagerResult result = agentsManagerAddApartmentToService(
								manager, email, "serveMe", apartment, 1 );
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS ) ;


	result = agentsManagerRemoveApartmentFromService( manager, email,"", 0 );
	ASSERT_TEST( result == AGENT_MANAGER_INVALID_PARAMETERS ) ;

	result = agentsManagerRemoveApartmentFromService( manager, email,"serveMee",
																			1 );
	ASSERT_TEST( result == AGENT_MANAGER_SERVICE_NOT_EXISTS ) ;

	result = agentsManagerRemoveApartmentFromService( manager, email,"serveMe",
																		2 );
		ASSERT_TEST( result == AGENT_MANAGER_APARTMENT_NOT_EXISTS ) ;

	result = agentsManagerRemoveApartmentFromService( manager, mail,
																"serveMe", 1 );
	ASSERT_TEST( result == AGENT_MANAGER_AGENT_NOT_EXISTS) ;

	result = agentsManagerRemoveApartmentFromService( manager, email,
																"serveMe", 1 );
	ASSERT_TEST( result == AGENT_MANAGER_SUCCESS) ;

	serviceDestroy(service);
	agentsManagerDestroy(manager);
	agentDestroy(agent);
	return true;
}




