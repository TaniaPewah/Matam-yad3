/*
 * mtmService_test.c
 *
 *  Created on: May 8, 2016
 *      Author: Tania
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "mtmService.h"

static bool testMtmServiceCreate();
static bool testMtmServiceAddAgent();
static bool testMtmServiceRemoveAgent();
static bool testMtmServiceAddServiceToAgent();
static bool testMtmServiceRemoveServiceFromAgent();
static bool testMtmServiceAddApartmentToAgent();
static bool testMtmServiceRemoveApartmentFromAgent();
static bool testMtmServiceAddClient();
static bool testMtmServiceRemoveClient();
static bool testMtmServiceClientPurchaseApartment();

int RunMtmServiceTest() {

	RUN_TEST(testMtmServiceCreate);
	RUN_TEST(testMtmServiceAddAgent);
	RUN_TEST(testMtmServiceRemoveAgent);
	RUN_TEST(testMtmServiceAddServiceToAgent);
	RUN_TEST(testMtmServiceRemoveServiceFromAgent);
	RUN_TEST(testMtmServiceAddApartmentToAgent);
	RUN_TEST(testMtmServiceRemoveApartmentFromAgent);
	RUN_TEST(testMtmServiceAddClient);
	RUN_TEST(testMtmServiceRemoveClient);
	RUN_TEST(testMtmServiceClientPurchaseApartment);
	return 0;
}

static bool testMtmServiceCreate() {
	MTMService service = mtmServiceCreate();
	ASSERT_TEST(service != NULL);
	mtmServiceDestroy(service);
	return true;
}
static bool testMtmServiceAddAgent() {
	MTMService service = mtmServiceCreate();

	ASSERT_TEST(mtmServiceAddAgent(NULL, "baba@ganosh", "tania", 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddAgent(service, NULL, "tania", 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddAgent(service, "baba@ganosh", NULL, 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddAgent(service, "babaganosh", "tania", 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddAgent(service, "baba@ganosh", "tania", 122) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddAgent(service, "baba@ganosh", "tania", -22) ==
			MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(mtmServiceAddAgent(service, "baba@ganosh", "tania", 5) ==
					MTM_SERVICE_SUCCESS);
	ASSERT_TEST(mtmServiceAddAgent(service, "baba@ganosh", "tadnia", 1) ==
			MTM_SERVICE_EMAIL_ALREADY_EXISTS);

	mtmServiceDestroy(service);
	return true;
}
static bool testMtmServiceRemoveAgent() {
	MTMService service = mtmServiceCreate();

	ASSERT_TEST(mtmServiceRemoveAgent(NULL, "baba@ganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceRemoveAgent(service, NULL) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceRemoveAgent(service, "babaganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceRemoveAgent(service, "ba@baganosh") ==
			MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(mtmServiceRemoveAgent(service, "baba@ganosh") ==
				MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	mtmServiceAddClient(service, "baba@ganosh", 1, 1, 155);

	ASSERT_TEST(mtmServiceRemoveAgent(service, "ba@baganosh") ==
				MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

		ASSERT_TEST(mtmServiceRemoveAgent(service, "baba@ganosh") ==
				MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	mtmServiceAddAgent(service, "baa@ganosh", "tania", 5);

	ASSERT_TEST(mtmServiceRemoveAgent(service, "ba@aganosh") ==
				MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(mtmServiceRemoveAgent(service, "baa@ganosh") ==
			MTM_SERVICE_SUCCESS);

	mtmServiceDestroy(service);
	return true;
}
static bool testMtmServiceAddServiceToAgent() {
	MTMService service = mtmServiceCreate();
	mtmServiceAddAgent(service, "baba@ganosh", "tania", 5);
	ASSERT_TEST(mtmServiceAddServiceToAgent(NULL, "baba@ganosh", "serveMe",
				20) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddServiceToAgent(service, NULL, "serveMe",
				20) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "baba@ganosh", NULL,
				20) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				0) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				-9) ==	MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "baa@ganosh", "serveMe",
				20) ==	MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "bab@aganosh", "serveMe",
				20) ==	MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	mtmServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "ba@ganosh", "serveMe",
			20) ==	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				20) ==	MTM_SERVICE_SUCCESS);
	ASSERT_TEST(mtmServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				20) ==	MTM_SERVICE_APARTMENT_SERVICE_ALREADY_EXISTS);

	mtmServiceDestroy(service);
	return true;
}
static bool testMtmServiceRemoveServiceFromAgent() {
	MTMService service = mtmServiceCreate();
	mtmServiceAddAgent(service, "baba@ganosh", "tania", 5);

	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, "baba@ganosh",
				"serveMe") == MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	mtmServiceAddServiceToAgent(service, "baba@ganosh", "serveMe", 20);
	mtmServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(NULL, "baba@ganosh",
				"serveMe") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, NULL,
				"serveMe") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, "baba@ganosh",
				NULL) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, "babaganosh",
				"serveMe") == MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, "baba@ganos",
				"serveMe") == MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, "ba@ganosh",
				"serveMe") == MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, "baba@ganosh",
				"serveMe") == MTM_SERVICE_SUCCESS);

	ASSERT_TEST(mtmServiceRemoveServiceFromAgent(service, "baba@ganosh",
			"serveMe") == MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	mtmServiceDestroy(service);
	return true;
}

static bool testMtmServiceAddApartmentToAgent(){

	MTMService service = mtmServiceCreate();
	mtmServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	mtmServiceAddServiceToAgent( service, "baba@ganosh", "serveMe", 2 );
	mtmServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST( mtmServiceAddApartmentToAgent(NULL, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, NULL,
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			NULL, 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "babaganosh",
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", -1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, -3, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, -2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, NULL) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "wd") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "weee") == MTM_SERVICE_INVALID_PARAMETERS);


	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "b@ganosh",
		"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "ba@ganosh",
		"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
				"seeMe", 1, 100, 1, 2, "we") ==
						MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_SUCCESS);

	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
	"serveMe", 1, 100, 2, 2, "weee") == MTM_SERVICE_APARTMENT_ALREADY_EXISTS);

	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
				"serveMe", 2, 100, 1, 2, "we") == MTM_SERVICE_SUCCESS);
	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 3, 100, 1, 2, "we") == MTM_SERVICE_APARTMENT_SERVICE_FULL);

	mtmServiceDestroy( service );
	return true;
}

static bool testMtmServiceRemoveApartmentFromAgent() {

	MTMService service = mtmServiceCreate();
	mtmServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	mtmServiceAddServiceToAgent( service, "baba@ganosh", "serveMe", 2 );
	mtmServiceAddClient(service, "ba@ganosh", 1, 1, 22);
	mtmServiceAddApartmentToAgent(service, "baba@ganosh",
					"serveMe", 2, 100, 1, 2, "we");

	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(NULL, "baba@ganosh",
						"serveMe", 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, NULL,
						"serveMe", 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "babaganosh",
						"serveMe", 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						NULL, 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", -1) == MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "bxxa@ganosh",
						"serveMe", 2) == MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "ba@ganosh",
						"serveMe", 2) == MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "baba@ganosh",
				"serve", 2) == MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", 3) == MTM_SERVICE_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", 2) == MTM_SERVICE_SUCCESS);
	ASSERT_TEST( mtmServiceRemoveApartmentFromAgent(service, "baba@ganosh",
					"serveMe", 2) == MTM_SERVICE_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST( mtmServiceAddApartmentToAgent(service, "baba@ganosh",
						"serveMe", 2, 100, 1, 2, "we") == MTM_SERVICE_SUCCESS);

	mtmServiceDestroy( service );
	return true;
}
static bool testMtmServiceAddClient() {
	MTMService service = mtmServiceCreate();

	ASSERT_TEST( mtmServiceAddClient( NULL, "ba@ganosh", 1, 1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( mtmServiceAddClient( service, NULL, 1, 1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( mtmServiceAddClient( service, "ba@ganosh", -1, 1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( mtmServiceAddClient( service, "ba@ganosh", 1, -1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( mtmServiceAddClient( service, "ba@ganosh", 1, 1, -22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( mtmServiceAddClient( service, "baganosh", 1, 1, -22 ) ==
				MTM_SERVICE_INVALID_PARAMETERS );

	ASSERT_TEST( mtmServiceAddClient( service, "ba@ganosh", 1, 1, 22 ) ==
					MTM_SERVICE_SUCCESS );
	ASSERT_TEST( mtmServiceAddClient( service, "ba@ganosh", 1, 1, 22 ) ==
			MTM_SERVICE_EMAIL_ALREADY_EXISTS );

	mtmServiceDestroy( service );
	return true;
}
static bool testMtmServiceRemoveClient() {
	MTMService service = mtmServiceCreate();
	mtmServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	mtmServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST( mtmServiceRemoveClient( NULL, "ba@ganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( mtmServiceRemoveClient( service, "baganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceRemoveClient( service, NULL) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( mtmServiceRemoveClient( service, "baga@nosh") ==
			MTM_SERVICE_EMAIL_DOES_NOT_EXIST);


	ASSERT_TEST( mtmServiceRemoveClient( service, "baba@ganosh") ==
			MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST( mtmServiceRemoveClient( service, "ba@ganosh") ==
			MTM_SERVICE_SUCCESS );

	ASSERT_TEST( mtmServiceRemoveClient( service, "ba@ganosh") ==
			MTM_SERVICE_EMAIL_DOES_NOT_EXIST );

	mtmServiceDestroy( service );
	return true;
}
static bool testMtmServiceClientPurchaseApartment() {

	MTMService service = mtmServiceCreate();
	mtmServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	mtmServiceAddClient(service, "ba@ganosh", 2, 2, 1000);
	mtmServiceAddServiceToAgent( service, "baba@ganosh", "serveMe", 3 );

	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
	"ba@ganosh","baba@ganosh","serveMe", 1) == MTM_SERVICE_APARTMENT_DOES_NOT_EXIST );

	mtmServiceAddApartmentToAgent(service, "baba@ganosh",
						"serveMe", 1, 100, 1, 2, "we");

	ASSERT_TEST(mtmServiceClientPurchaseApartment(NULL,
				"ba@ganosh","baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
				NULL,"baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
				"ba@ganosh",NULL,"serveMe", 1) ==
						MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
				"ba@ganosh","baba@ganosh",NULL, 1) ==
						MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
				"baganosh","baba@ganosh","serveMe", 1) ==
							MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
				"ba@ganosh","babaganosh","serveMe", 1) ==
							MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
				"ba@ganosh","baba@ganosh","serveMe", -1) ==
								MTM_SERVICE_INVALID_PARAMETERS );



	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
			"b@ganosh","baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
				"baba@ganosh","nba@ganosh","serveMe", 1) ==
						MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveggMe", 1) ==
					MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST );


	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 7) ==
					MTM_SERVICE_APARTMENT_DOES_NOT_EXIST );

	ASSERT_TEST(mtmServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_PURCHASE_WRONG_PROPERTIES );

	mtmServiceAddApartmentToAgent(service, "baba@ganosh",
					"serveMe", 3, 100, 3, 3, "ewwwewwwe");

	ASSERT_TEST( mtmServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 3) == MTM_SERVICE_SUCCESS);

	mtmServiceDestroy( service );
	return true;
}
