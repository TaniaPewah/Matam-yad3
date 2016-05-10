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

static bool testYad3ServiceCreate();
static bool testYad3ServiceAddAgent();
static bool testYad3ServiceRemoveAgent();
static bool testYad3ServiceAddServiceToAgent();
static bool testYad3ServiceRemoveServiceFromAgent();
static bool testYad3ServiceAddApartmentToAgent();
static bool testYad3ServiceRemoveApartmentFromAgent();
static bool testYad3ServiceAddClient();
static bool testYad3ServiceRemoveClient();
static bool testYad3ServiceClientPurchaseApartment();

int RunYad3ServiceTest() {

	RUN_TEST(testYad3ServiceCreate);
	RUN_TEST(testYad3ServiceAddAgent);
	RUN_TEST(testYad3ServiceRemoveAgent);
	RUN_TEST(testYad3ServiceAddServiceToAgent);
	RUN_TEST(testYad3ServiceRemoveServiceFromAgent);
	RUN_TEST(testYad3ServiceAddApartmentToAgent);
	RUN_TEST(testYad3ServiceRemoveApartmentFromAgent);
	RUN_TEST(testYad3ServiceAddClient);
	RUN_TEST(testYad3ServiceRemoveClient);
	RUN_TEST(testYad3ServiceClientPurchaseApartment);
	return 0;
}

static bool testYad3ServiceCreate() {
	Yad3Service service = yad3ServiceCreate();
	ASSERT_TEST(service != NULL);
	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceAddAgent() {
	Yad3Service service = yad3ServiceCreate();

	ASSERT_TEST(yad3ServiceAddAgent(NULL, "baba@ganosh", "tania", 5) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, NULL, "tania", 5) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", NULL, 5) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "babaganosh", "tania", 5) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tania", 122) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tania", -22) ==
			YAD3_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tania", 5) ==
					YAD3_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceAddAgent(service, "baba@ganosh", "tadnia", 1) ==
			YAD3_SERVICE_EMAIL_ALREADY_EXISTS);

	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceRemoveAgent() {
	Yad3Service service = yad3ServiceCreate();

	ASSERT_TEST(yad3ServiceRemoveAgent(NULL, "baba@ganosh") ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, NULL) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, "babaganosh") ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, "ba@baganosh") ==
			YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceRemoveAgent(service, "baba@ganosh") ==
				YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);

	yad3ServiceAddClient(service, "baba@ganosh", 1, 1, 155);

	ASSERT_TEST(yad3ServiceRemoveAgent(service, "ba@baganosh") ==
				YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);

		ASSERT_TEST(yad3ServiceRemoveAgent(service, "baba@ganosh") ==
				YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	yad3ServiceAddAgent(service, "baa@ganosh", "tania", 5);

	ASSERT_TEST(yad3ServiceRemoveAgent(service, "ba@aganosh") ==
				YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(yad3ServiceRemoveAgent(service, "baa@ganosh") ==
			YAD3_SERVICE_SUCCESS);

	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceAddServiceToAgent() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 5);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(NULL, "baba@ganosh", "serveMe",
				20) ==	YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, NULL, "serveMe",
				20) ==	YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", NULL,
				20) ==	YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				0) ==	YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				-9) ==	YAD3_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baa@ganosh", "serveMe",
				20) ==	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "bab@aganosh", "serveMe",
				20) ==	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "ba@ganosh", "serveMe",
			20) ==	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				20) ==	YAD3_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe",
				20) ==	YAD3_SERVICE_APARTMENT_SERVICE_ALREADY_EXISTS);

	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceRemoveServiceFromAgent() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 5);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
				"serveMe") == YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe", 20);
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(NULL, "baba@ganosh",
				"serveMe") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, NULL,
				"serveMe") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
				NULL) == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "babaganosh",
				"serveMe") == YAD3_SERVICE_INVALID_PARAMETERS);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganos",
				"serveMe") == YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "ba@ganosh",
				"serveMe") == YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
				"serveMe") == YAD3_SERVICE_SUCCESS);

	ASSERT_TEST(yad3ServiceRemoveServiceFromAgent(service, "baba@ganosh",
			"serveMe") == YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);

	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceAddApartmentToAgent(){

	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 1);
	yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe", 2);
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);

	ASSERT_TEST(yad3ServiceAddApartmentToAgent(NULL, "baba@ganosh",
			"serveMe", 1, 100, 1, 2, "we") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, NULL,
			"serveMe", 1, 100, 1, 2, "we") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
			NULL, 1, 100, 1, 2, "we") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "babaganosh",
		"serveMe", 1, 100, 1, 2, "we") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", -1, 100, 1, 2, "we") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 1, -3, 1, 2, "we") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 1, 100, 1, -2, "we") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 1, 100, 1, 2, NULL) == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 1, 100, 1, 2, "wd") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 1, 100, 1, 2, "weee") == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "b@ganosh",
		"serveMe", 1, 100, 1, 2, "we") == YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "ba@ganosh",
		"serveMe", 1, 100, 1, 2, "we")
			== YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"seeMe", 1, 100, 1, 2, "we") ==
			YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 1, 100, 1, 2, "we") == YAD3_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 1, 100, 2, 2, "weee")
			== YAD3_SERVICE_APARTMENT_ALREADY_EXISTS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 2, 100, 1, 2, "we") == YAD3_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
		"serveMe", 3, 100, 1, 2, "we") == YAD3_SERVICE_APARTMENT_SERVICE_FULL);
	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceRemoveApartmentFromAgent() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 1);
	yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe", 2);
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);
	yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
					"serveMe", 2, 100, 1, 2, "we");
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(NULL, "baba@ganosh",
						"serveMe", 2) == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, NULL,
						"serveMe", 2) == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "babaganosh",
						"serveMe", 2) == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						NULL, 2) == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", -1) == YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "bxxa@ganosh",
						"serveMe", 2) == YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "ba@ganosh",
						"serveMe", 2) == YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
				"serve", 2) == YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", 3) == YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
						"serveMe", 2) == YAD3_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceRemoveApartmentFromAgent(service, "baba@ganosh",
					"serveMe", 2) == YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
						"serveMe", 2, 100, 1, 2, "we") == YAD3_SERVICE_SUCCESS);
	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceAddClient() {
	Yad3Service service = yad3ServiceCreate();
	ASSERT_TEST(yad3ServiceAddClient(NULL, "ba@ganosh", 1, 1, 22) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddClient(service, NULL, 1, 1, 22) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddClient(service, "ba@ganosh", -1, 1, 22) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddClient(service, "ba@ganosh", 1, -1, 22) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddClient(service, "ba@ganosh", 1, 1, -22) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddClient(service, "baganosh", 1, 1, -22) ==
				YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22) ==
					YAD3_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22) ==
			YAD3_SERVICE_EMAIL_ALREADY_EXISTS);
	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceRemoveClient() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 1);
	yad3ServiceAddClient(service, "ba@ganosh", 1, 1, 22);
	ASSERT_TEST(yad3ServiceRemoveClient(NULL, "ba@ganosh") ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveClient(service, "baganosh") ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveClient(service, NULL) ==
			YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceRemoveClient(service, "baga@nosh") ==
			YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceRemoveClient(service, "baba@ganosh") ==
			YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(yad3ServiceRemoveClient(service, "ba@ganosh") ==
			YAD3_SERVICE_SUCCESS);
	ASSERT_TEST(yad3ServiceRemoveClient(service, "ba@ganosh") ==
			YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	yad3ServiceDestroy(service);
	return true;
}

static bool testYad3ServiceClientPurchaseApartment() {
	Yad3Service service = yad3ServiceCreate();
	yad3ServiceAddAgent(service, "baba@ganosh", "tania", 1);
	yad3ServiceAddClient(service, "ba@ganosh", 2, 2, 1000);
	yad3ServiceAddServiceToAgent(service, "baba@ganosh", "serveMe", 3);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
	"ba@ganosh","baba@ganosh","serveMe", 1) ==
			YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST);
	yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
						"serveMe", 1, 100, 1, 2, "we");
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(NULL,
				"ba@ganosh","baba@ganosh","serveMe", 1) ==
					YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				NULL,"baba@ganosh","serveMe", 1) ==
					YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh",NULL,"serveMe", 1) ==
						YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh","baba@ganosh",NULL, 1) ==
						YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"baganosh","baba@ganosh","serveMe", 1) ==
							YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh","babaganosh","serveMe", 1) ==
							YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"ba@ganosh","baba@ganosh","serveMe", -1) ==
								YAD3_SERVICE_INVALID_PARAMETERS);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"b@ganosh","baba@ganosh","serveMe", 1) ==
					YAD3_SERVICE_EMAIL_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
				"baba@ganosh","nba@ganosh","serveMe", 1) ==
						YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveggMe", 1) ==
					YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 7) ==
					YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST);
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 1) ==
					YAD3_SERVICE_PURCHASE_WRONG_PROPERTIES);
	yad3ServiceAddApartmentToAgent(service, "baba@ganosh",
					"serveMe", 3, 100, 3, 3, "ewwwewwwe");
	ASSERT_TEST(yad3ServiceClientPurchaseApartment(service,
			"ba@ganosh","baba@ganosh","serveMe", 3) == YAD3_SERVICE_SUCCESS);
	yad3ServiceDestroy(service);
	return true;
}
