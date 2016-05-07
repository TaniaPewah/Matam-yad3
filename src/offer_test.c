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
	RUN_TEST(testOfferGeters);
	RUN_TEST(testOfferCompareAndEqals);
	RUN_TEST(testOfferCopy);
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
		ASSERT_TEST(offerCreate(client, agent, serivce_name, 12, 5000, &offer)
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
	char* client_b_address = "client.braint@mail.com";
	char* agent_address = "agent@mail.com";
	Email client, client_b, agent;
	ASSERT_TEST(emailCreate(client_address, &client) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(client_b_address, &client_b) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(agent_address, &agent) == EMAIL_SUCCESS);
	char* serivce_name = "my simple service";
	Offer first_offer, second_offer, thired_offer;
	ASSERT_TEST(offerCreate(client, agent, serivce_name, 12, 5000,
			&first_offer) == OFFER_SUCCESS);
	ASSERT_TEST(first_offer != NULL);
	ASSERT_TEST(offerCreate(client, agent, serivce_name, 12, 6500,
			&second_offer) == OFFER_SUCCESS);
	ASSERT_TEST(second_offer != NULL);
	ASSERT_TEST(offerCreate(client_b, agent, serivce_name, 12, 6500,
			&thired_offer) == OFFER_SUCCESS);
	ASSERT_TEST(second_offer != NULL);
	ASSERT_TEST(offerAreOverlapping(first_offer, NULL) == false);
	ASSERT_TEST(offerAreOverlapping(NULL, first_offer) == false);
	ASSERT_TEST(offerAreOverlapping(first_offer, second_offer) == true);
	ASSERT_TEST(offerAreOverlapping(thired_offer, first_offer) == false);
	ASSERT_TEST(offerAreOverlapping(thired_offer, second_offer) == false);
	offerDestroy(thired_offer);
	offerDestroy(second_offer);
	offerDestroy(first_offer);
	emailDestroy(client);
	emailDestroy(agent);
	return true;
}

/**
 * Test method emailToString
 */
static bool testOfferGeters() {
	char* client_address = "client@mail.com";
	char* agent_address = "agent@mail.com";
	Email client, agent;
	ASSERT_TEST(emailCreate(client_address, &client) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(agent_address, &agent) == EMAIL_SUCCESS);
	char* serivce_name = "my simple service";
	Offer offer;
	ASSERT_TEST(offerCreate(client, agent, serivce_name, 12, 5000, &offer)
			== OFFER_SUCCESS);
	ASSERT_TEST(offer != NULL);
	ASSERT_TEST(offerGetApartmentId(NULL) == NO_OFFER_VAL);
	ASSERT_TEST(offerGetApartmentId(offer) == 12);
	ASSERT_TEST(offerGetPrice(NULL) == NO_OFFER_VAL);
	ASSERT_TEST(offerGetPrice(offer) == 5000);
	ASSERT_TEST(offerGetAgentEmail(NULL) == NULL);
	ASSERT_TEST(emailAreEqual(offerGetAgentEmail(offer), agent)
		== true);
	ASSERT_TEST(offerGetClientEmail(NULL) == NULL);
	ASSERT_TEST(emailAreEqual(offerGetClientEmail(offer), client)
		== true);
	ASSERT_TEST(offerGetServiceName(NULL) == NULL);
	ASSERT_TEST(areStringsEqual(offerGetServiceName(offer), serivce_name)
		== true);
	offerDestroy(offer);
	emailDestroy(client);
	emailDestroy(agent);
	return true;
}
