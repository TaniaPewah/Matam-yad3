/*
 * yad3Service_test.c
 *
 *  Created on: May 8, 2016
 *      Author: Tania
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "yad3Service.h"

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
	Yad3Service service = yad3ServiceCreate();
	ASSERT_TEST(service != NULL);
	yad3ServiceDestroy(service);
	return true;
}
static bool testMtmServiceAddAgent() {
	Yad3Service service = yad3ServiceCreate();

	ASSERT_TEST(yad3ServiceAddAgent(NULL, "baba@ganosh", "tania", 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, NULL, "tania", 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", NULL, 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "babaganosh", "tania", 5) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tania", 122) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tania", -22) ==
			MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tania", 5) ==
					MTM_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tadnia", 1) ==
			MTM_SERVICE_EMAIL_ALREADY_EXISTS);

	yad3ServiceDestroy(service);
	return true;
}
static bool testMtmServiceRemoveAgent() {
	Yad3Service service = yad3ServiceCreate();

	ASSERT_TEST(yad3ServiceRemoveAgent(NULL, "baba@ganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, NULL) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, "babaganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, "ba@baganosh") ==
			MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, "baba@ganosh") ==
				MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	yad3ServiceAddClient(service, "baba@ganosh", 1, 1, 155);

	ASSERT_TEST(yad3ServiceRemoveAgent(service, "ba@baganosh") ==
				MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

		ASSERT_TEST(yad3ServiceRemoveAgent(service, "baba@ganosh") ==
				MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	yad3ServiceAddAgent(service, "baa@ganosh", "tania", 5);

	ASSERT_TEST(yad3ServiceRemoveAgent(service, "ba@aganosh") ==
				MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(yad3ServiceRemoveAgent(service, "baa@ganosh") ==
			MTM_SERVICE_SUCCESS);

	yad3ServiceDestroy(service);
	return true;
}
static bool testMtmServiceAddServiceToAgent() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 5);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(NULL, "baba@ganosh", "serveMe",
				20) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, NULL, "serveMe",
				20) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", NULL,
				20) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				0) ==	MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				-9) ==	MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baa@ganosh", "serveMe",
				20) ==	MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "bab@aganosh", "serveMe",
				20) ==	MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "ba@ganosh", "serveMe",
			20) ==	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				20) ==	MTM_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				20) ==	MTM_SERVICE_APARTMENT_SERVICE_ALREADY_EXISTS);

	yad3ServiceDestroy(service);
	return true;
}
static bool testMtmServiceRemoveServiceFromAgent() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 5);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
				"serveMe") == MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe", 20);
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(NULL, "baba@ganosh",
				"serveMe") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, NULL,
				"serveMe") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
				NULL) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "babaganosh",
				"serveMe") == MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganos",
				"serveMe") == MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "ba@ganosh",
				"serveMe") == MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
				"serveMe") == MTM_SERVICE_SUCCESS);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
			"serveMe") == MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	yad3ServiceDestroy(service);
	return true;
}

static bool testMtmServiceAddApartmentToAgent(){

	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	yad3ServiceAddServiceToAgent( service, "baba@ganosh", "serveMe", 2 );
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST( yad3ServiceAddApartmentToAgent(NULL, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, NULL,
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			NULL, 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "babaganosh",
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", -1, 100, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, -3, 1, 2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, -2, "we") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, NULL) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "wd") == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "weee") == MTM_SERVICE_INVALID_PARAMETERS);


	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "b@ganosh",
		"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "ba@ganosh",
		"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
				"seeMe", 1, 100, 1, 2, "we") ==
						MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "we") == MTM_SERVICE_SUCCESS);

	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
	"serveMe", 1, 100, 2, 2, "weee") == MTM_SERVICE_APARTMENT_ALREADY_EXISTS);

	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
				"serveMe", 2, 100, 1, 2, "we") == MTM_SERVICE_SUCCESS);
	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 3, 100, 1, 2, "we") == MTM_SERVICE_APARTMENT_SERVICE_FULL);

	yad3ServiceDestroy( service );
	return true;
}

static bool testMtmServiceRemoveApartmentFromAgent() {

	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	yad3ServiceAddServiceToAgent( service, "baba@ganosh", "serveMe", 2 );
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);
	yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
					"serveMe", 2, 100, 1, 2, "we");

	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(NULL, "baba@ganosh",
						"serveMe", 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, NULL,
						"serveMe", 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "babaganosh",
						"serveMe", 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						NULL, 2) == MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", -1) == MTM_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "bxxa@ganosh",
						"serveMe", 2) == MTM_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "ba@ganosh",
						"serveMe", 2) == MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
				"serve", 2) == MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", 3) == MTM_SERVICE_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", 2) == MTM_SERVICE_SUCCESS);
	ASSERT_TEST( yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
					"serveMe", 2) == MTM_SERVICE_APARTMENT_DOES_NOT_EXIST);

	ASSERT_TEST( yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
						"serveMe", 2, 100, 1, 2, "we") == MTM_SERVICE_SUCCESS);

	yad3ServiceDestroy( service );
	return true;
}
static bool testMtmServiceAddClient() {
	Yad3Service service = yad3ServiceCreate();

	ASSERT_TEST( yad3ServiceAddClient( NULL, "ba@ganosh", 1, 1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( yad3ServiceAddClient( service, NULL, 1, 1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( yad3ServiceAddClient( service, "ba@ganosh", -1, 1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( yad3ServiceAddClient( service, "ba@ganosh", 1, -1, 22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( yad3ServiceAddClient( service, "ba@ganosh", 1, 1, -22 ) ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( yad3ServiceAddClient( service, "baganosh", 1, 1, -22 ) ==
				MTM_SERVICE_INVALID_PARAMETERS );

	ASSERT_TEST( yad3ServiceAddClient( service, "ba@ganosh", 1, 1, 22 ) ==
					MTM_SERVICE_SUCCESS );
	ASSERT_TEST( yad3ServiceAddClient( service, "ba@ganosh", 1, 1, 22 ) ==
			MTM_SERVICE_EMAIL_ALREADY_EXISTS );

	yad3ServiceDestroy( service );
	return true;
}
static bool testMtmServiceRemoveClient() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST( yad3ServiceRemoveClient( NULL, "ba@ganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST( yad3ServiceRemoveClient( service, "baganosh") ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceRemoveClient( service, NULL) ==
			MTM_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST( yad3ServiceRemoveClient( service, "baga@nosh") ==
			MTM_SERVICE_EMAIL_DOES_NOT_EXIST);


	ASSERT_TEST( yad3ServiceRemoveClient( service, "baba@ganosh") ==
			MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST( yad3ServiceRemoveClient( service, "ba@ganosh") ==
			MTM_SERVICE_SUCCESS );

	ASSERT_TEST( yad3ServiceRemoveClient( service, "ba@ganosh") ==
			MTM_SERVICE_EMAIL_DOES_NOT_EXIST );

	yad3ServiceDestroy( service );
	return true;
}
static bool testMtmServiceClientPurchaseApartment() {

	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent( service, "baba@ganosh", "tania", 1 );
	yad3ServiceAddClient(service, "ba@ganosh", 2, 2, 1000);
	yad3ServiceAddServiceToAgent( service, "baba@ganosh", "serveMe", 3 );

	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
	"ba@ganosh","baba@ganosh","serveMe", 1) == MTM_SERVICE_APARTMENT_DOES_NOT_EXIST );

	yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
						"serveMe", 1, 100, 1, 2, "we");

	ASSERT_TEST(yad3ServiceClientPurchaseApartment(NULL,
				"ba@ganosh","baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				NULL,"baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh",NULL,"serveMe", 1) ==
						MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh","baba@ganosh",NULL, 1) ==
						MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"baganosh","baba@ganosh","serveMe", 1) ==
							MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh","babaganosh","serveMe", 1) ==
							MTM_SERVICE_INVALID_PARAMETERS );
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh","baba@ganosh","serveMe", -1) ==
								MTM_SERVICE_INVALID_PARAMETERS );



	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"b@ganosh","baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"baba@ganosh","nba@ganosh","serveMe", 1) ==
						MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveggMe", 1) ==
					MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST );


	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 7) ==
					MTM_SERVICE_APARTMENT_DOES_NOT_EXIST );

	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 1) ==
					MTM_SERVICE_PURCHASE_WRONG_PROPERTIES );

	yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
					"serveMe", 3, 100, 3, 3, "ewwwewwwe");

	ASSERT_TEST( yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 3) == MTM_SERVICE_SUCCESS);

	yad3ServiceDestroy( service );
	return true;
}
