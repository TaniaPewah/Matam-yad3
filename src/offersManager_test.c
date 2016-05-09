/*
 * offersManager_test.c
 *
 *  Created on: May 9, 2016
 *      Author: Tania
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "offersManager.h"
#include "test_utilities.h"
#include "email.h"
#include "agent.h"
#include "client.h"


static bool testOfferManagerCreate();
static bool testOffersMenagerRemoveAllEmailOffers();
static bool testOffersMenagerRemoveAllServiceOffers();
static bool testOffersMenagerRemoveAllApartmentOffers();
static bool testOffersManagerOfferExist();
static bool testOffersManagerAddOffer();


int RunOffersManagerTest() {

	RUN_TEST(testOfferManagerCreate);
	RUN_TEST(testOffersMenagerRemoveAllEmailOffers);
	RUN_TEST(testOffersMenagerRemoveAllServiceOffers);
	RUN_TEST(testOffersMenagerRemoveAllApartmentOffers);
	RUN_TEST(testOffersManagerOfferExist);
	RUN_TEST(testOffersManagerAddOffer);

	return 0;
}

static bool testOfferManagerCreate(){
	OffersManager manager = offersManagerCreate();
	ASSERT_TEST( manager != NULL);
	offersManagerDestroy( manager );
	return true;
}
static bool testOffersMenagerRemoveAllEmailOffers(){
	OffersManager manager = offersManagerCreate();
	Email email;
	emailCreate( "tania@m", &email );
	ASSERT_TEST( offersMenagerRemoveAllEmailOffers( NULL, email ) ==
			OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersMenagerRemoveAllEmailOffers( manager, NULL ) ==
			OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersMenagerRemoveAllEmailOffers( manager, email ) ==
			OFFERS_MANAGER_SUCCESS );

	emailDestroy(email);
	offersManagerDestroy( manager );
	return true;
}
static bool testOffersMenagerRemoveAllServiceOffers(){
	OffersManager manager = offersManagerCreate();
	Email email;
	emailCreate( "tania@m", &email );
	ASSERT_TEST( offersMenagerRemoveAllServiceOffers( NULL, email, "b" ) ==
			OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersMenagerRemoveAllServiceOffers( manager, NULL, "n" ) ==
			OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersMenagerRemoveAllServiceOffers( manager, email, NULL ) ==
			OFFERS_MANAGER_NULL_PARAMETERS );

	ASSERT_TEST( offersMenagerRemoveAllServiceOffers( manager, email, "m" ) ==
			OFFERS_MANAGER_SUCCESS );


	emailDestroy(email);
	offersManagerDestroy( manager );
	return true;
}
static bool testOffersMenagerRemoveAllApartmentOffers(){
	OffersManager manager = offersManagerCreate();
		Email email;
		emailCreate( "client@m", &email );
		Email mail;
		emailCreate( "agent@m", &mail );
		Agent agent;
		agentCreate(mail,"comp", 1, &agent);
		Client client;
		clientCreate(email, 1, 1, 30, &client);
		agentAddService( agent, "serveMe", 3);

		agentAddApartmentToService(agent, "serveMe",1,100,1,2,"we");
		offersManagerAddOffer( manager,email, mail, "serveMe",
					1, 1000 );
		ASSERT_TEST( offersManagerOfferExist(manager,email,mail,"serveMe", 1)
				== true );
		ASSERT_TEST( offersMenagerRemoveAllApartmentOffers(NULL,mail, "serveMe",
				1) == OFFERS_MANAGER_NULL_PARAMETERS);
		ASSERT_TEST( offersMenagerRemoveAllApartmentOffers(manager,NULL, "serveMe",
						1) == OFFERS_MANAGER_NULL_PARAMETERS);
		ASSERT_TEST( offersMenagerRemoveAllApartmentOffers(manager,mail, NULL,
						1) == OFFERS_MANAGER_NULL_PARAMETERS);

		ASSERT_TEST( offersMenagerRemoveAllApartmentOffers(manager,mail, "serveMe",
						1) == OFFERS_MANAGER_SUCCESS);


		ASSERT_TEST( offersManagerOfferExist(manager,email,mail,"serveMe", 1)
						== false );
		agentDestroy(agent);
		clientDestroy(client);
		emailDestroy(email);
		offersManagerDestroy( manager );
		return true;
}
static bool testOffersManagerOfferExist(){
	OffersManager manager = offersManagerCreate();
	Email email;
	emailCreate( "client@m", &email );
	Email mail;
	emailCreate( "agent@m", &mail );
	Agent agent;
	agentCreate(mail,"comp", 1, &agent);
	Client client;
	clientCreate(email, 1, 1, 30, &client);
	agentAddService( agent, "serveMe", 3);
	ASSERT_TEST( offersManagerOfferExist(manager,mail,email,"serveMe", 1)
			== false );
	agentAddApartmentToService(agent, "serveMe",1,100,1,2,"we");
	offersManagerAddOffer( manager,email, mail, "serveMe",
				1, 1000 );
	ASSERT_TEST( offersManagerOfferExist(manager,email,mail,"serveMe", 1)
			== true );
	agentDestroy(agent);
	clientDestroy(client);
	emailDestroy(email);
	offersManagerDestroy( manager );
	return true;
}
static bool testOffersManagerAddOffer(){
	OffersManager manager = offersManagerCreate();
	Email email;
	emailCreate( "tania@m", &email );
	Email mail;
	emailCreate( "ta@m", &mail );
	Agent agent;
	agentCreate(email,"comp", 1, &agent);
	Client client;
	clientCreate(mail, 1, 1, 30, &client);
	agentAddService( agent, "serveMe", 3);
	agentAddApartmentToService(agent, "serveMe",1,100,1,2,"we");

	ASSERT_TEST( offersManagerAddOffer( NULL,mail, email, "serveMe",
				1, 100 ) ==	OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersManagerAddOffer( manager,NULL, email, "serveMe",
				1, 100 ) ==	OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersManagerAddOffer( manager,mail, NULL, "serveMe",
				1, 100 ) ==	OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersManagerAddOffer( manager,mail, email, NULL,
				1, 100 ) ==	OFFERS_MANAGER_NULL_PARAMETERS );
	ASSERT_TEST( offersManagerAddOffer( manager,mail, email, "serveMe",
				-2, 100 ) == OFFERS_MANAGER_INVALID_PARAMETERS );
	ASSERT_TEST( offersManagerAddOffer( manager,mail, email, "serveMe",
				3, -100 ) == OFFERS_MANAGER_INVALID_PARAMETERS );

	ASSERT_TEST( offersManagerAddOffer( manager,mail, email, "serveMe",
			1, 100 ) ==	OFFERS_MANAGER_SUCCESS );

	agentDestroy(agent);
	clientDestroy(client);
	emailDestroy(email);
	offersManagerDestroy( manager );
	return true;
}

