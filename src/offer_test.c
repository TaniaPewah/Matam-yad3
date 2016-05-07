#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "utilities.h"
#include "email.h"
#include "offer.h"
#include "test_utilities.h"

static bool testOfferCreate();
static bool testOfferCopy();
static bool testOfferCompareAndEqals();
static bool testOfferGeters();

//int main() {
int RunOfferTest() {
	RUN_TEST(testOfferCreate);
	RUN_TEST(testOfferCompareAndEqals);
	RUN_TEST(testOfferCopy);
	RUN_TEST(testOfferGeters);
	return 0;
}

/**
 * Test create method
 */
static bool testOfferCreate() {
	char* client_address = "client@mail.com";
	char* agent_address = "agent@mail.com";
	Email client, agent;
	ASSERT_TEST(emailCreate(client_address, &client) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(agent_address, &agent) == EMAIL_SUCCESS);
	char* serivce_name = "my simple service";
	Offer offer;
	ASSERT_TEST(offerCreate(NULL, agent, serivce_name, 12, 5000, &offer) ==
			OFFER_NULL_PARAMETERS);
	ASSERT_TEST(offerCreate(client, NULL, serivce_name, 12, 5000, &offer) ==
			OFFER_NULL_PARAMETERS);
	ASSERT_TEST(offerCreate(agent, client, NULL, 12, 5000, &offer) ==
			OFFER_NULL_PARAMETERS);
	ASSERT_TEST(offerCreate(agent, client, serivce_name, 12, 5000, NULL) ==
			OFFER_NULL_PARAMETERS);
	ASSERT_TEST(offerCreate(agent, client, serivce_name, 12, 5000, &offer) ==
			OFFER_SUCCESS);
	ASSERT_TEST(offer != NULL);
	offerDestroy(offer);
	emailDestroy(client);
	emailDestroy(agent);
	return true;
}

/**
 * Test copy method
 */
static bool testOfferCopy() {
	char* client_address = "client@mail.com";
		char* agent_address = "agent@mail.com";
		Email client, agent;
		ASSERT_TEST(emailCreate(client_address, &client) == EMAIL_SUCCESS);
		ASSERT_TEST(emailCreate(agent_address, &agent) == EMAIL_SUCCESS);
		char* serivce_name = "my simple service";
		Offer offer, copy;
		ASSERT_TEST(offerCreate(agent, client, serivce_name, 12, 5000, &offer)
				== OFFER_SUCCESS);
		ASSERT_TEST(offer != NULL);
		ASSERT_TEST(offerCopy(NULL, NULL) == OFFER_NULL_PARAMETERS);
		ASSERT_TEST(offerCopy(offer, NULL) == OFFER_NULL_PARAMETERS);
		ASSERT_TEST(offerCopy(NULL, &copy) == OFFER_NULL_PARAMETERS);
		ASSERT_TEST(offerCopy(offer, &copy) == OFFER_SUCCESS);
		ASSERT_TEST(copy != NULL);
		ASSERT_TEST(emailAreEqual(offerGetAgentEmail(copy),agent) == true);
		ASSERT_TEST(emailAreEqual(offerGetClientEmail(copy),client) == true);
		ASSERT_TEST(areStringsEqual(offerGetServiceName(copy),serivce_name)
				== true);
		ASSERT_TEST(offerGetApartmentId(copy) == 12);
		ASSERT_TEST(offerGetPrice(copy) == 5000);
		offerDestroy(copy);
		offerDestroy(offer);
		emailDestroy(client);
		emailDestroy(agent);
	return true;
}

/**
 * Test Compare method and Equal method
 */
static bool testOfferCompareAndEqals() {
	char* client_address = "client@mail.com";
	char* agent_address = "agent@mail.com";
	Email client, agent;
	ASSERT_TEST(emailCreate(client_address, &client) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(agent_address, &agent) == EMAIL_SUCCESS);
	char* serivce_name = "my simple service";
	Offer offer;
	ASSERT_TEST(offerCreate(agent, client, serivce_name, 12, 5000, &offer)
			== OFFER_SUCCESS);
	ASSERT_TEST(offer != NULL);

	ASSERT_TEST(offerGetApartmentId(NULL) == NO_OFFER_VAL);
	ASSERT_TEST(offerGetApartmentId(NULL) == NO_OFFER_VAL);

	ASSERT_TEST(offerGetPrice(NULL) == NO_OFFER_VAL);

	offerDestroy(offer);
	emailDestroy(client);
	emailDestroy(agent);
	return true;
	return true;
}

/**
 * Test method emailToString
 */
static bool testOfferGeters() {
	char* address_1  = "aaa@gmail.com";
	char* address_2  = "bbb@gmail.com";
	Email mail_1 = NULL;
	Email mail_2 = NULL;
	char *new_address_1 = NULL;
	char *new_address_2 = NULL;
	ASSERT_TEST(emailCreate(address_1, &mail_1) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(address_2, &mail_2) == EMAIL_SUCCESS);
	ASSERT_TEST(emailToString(NULL) == NULL);
	new_address_1 = emailToString(mail_1);
	new_address_2 = emailToString(mail_2);
	ASSERT_TEST(new_address_1 != NULL);
	ASSERT_TEST(new_address_2 != NULL);
	ASSERT_TEST(strcmp(address_1, new_address_1) == 0);
	ASSERT_TEST(strcmp(address_2, new_address_2) == 0);
	ASSERT_TEST(strcmp(address_2, new_address_1) != 0);
	ASSERT_TEST(strcmp(address_1, new_address_2) != 0);
	free(new_address_1);
	free(new_address_2);
	emailDestroy(mail_1);
	emailDestroy(mail_2);
	return true;
}
