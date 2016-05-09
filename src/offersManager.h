#ifndef SRC_OFFERSMANAGER_H_
#define SRC_OFFERSMANAGER_H_

#include "offer.h"
#include "email.h"

typedef struct offerManager_t *OffersManager;

/**
* This type defines end codes for the methods.
*/
typedef enum {
	OFFERS_MANAGER_NULL_PARAMETERS = 0,
	OFFERS_MANAGER_INVALID_PARAMETERS = 1,
	OFFERS_MANAGER_OUT_OF_MEMORY = 2,
	OFFERS_MANAGER_SUCCESS = 3
} OfferManagerResult;

/**
* Allocates a new OfferManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new clients in case of success.
*/
OffersManager offersManagerCreate();

/**
* offerManagerDestroy: Deallocates an existing offers manager.
* Clears the element by using the stored free function.
*
* @param offer Target offer to be deallocated.
* If offer is NULL nothing will be done
*/
void offersManagerDestroy(OffersManager manager);

/*
 * offersMenagerRemoveAllConnectedOffers: Removes all the offers associated
 * with the given email.
*
* @param manager OffersManager to remove from add to.
* @param mail client or agent email address.
*
* @return
*
* 	OFFERS_MANAGER_NULL_PARAMETERS if manager or mail are NULL
*
* 	OFFERS_MANAGER_SUCCESS the offers removed successfully
*/
OfferManagerResult offersMenagerRemoveAllEmailOffers(OffersManager manager,
	Email mail);

/*
* offersMenagerRemoveAllServiceOffers: Removes all the offers that are under
* the given apartment service name and agent.
*
* @param manager OffersManager to remove from add to.
* @param mail agent email.
* @param service_name the apartment service name.
*
* @return
*
* 	OFFERS_MANAGER_NULL_PARAMETERS if manager, mail or service_name are NULL
*
* 	OFFERS_MANAGER_OUT_OF_MEMORY in case of memory allocation error
*
* 	OFFERS_MANAGER_SUCCESS the offers removed successfully
*/
OfferManagerResult offersMenagerRemoveAllServiceOffers(OffersManager manager,
	Email mail, char* service_name);

/*
* offersMenagerRemoveOffer: Removes spesific offer that between client and
* agent.
*
* @param manager OffersManager to remove from add to.
* @param mail client email.
* @param mail agent email.
*
* @return
*
* 	OFFERS_MANAGER_NULL_PARAMETERS if manager, mail or service_name are NULL
*
* 	OFFERS_MANAGER_OUT_OF_MEMORY in case of memory allocation error
*
* 	OFFERS_MANAGER_SUCCESS the offers removed successfully
*/
OfferManagerResult offersMenagerRemoveOffer(OffersManager manager,
	Email client, Email agent);

/*
* offersMenagerRemoveAllApartmentOffers: Removes all the offers that are on
* the given apartment id.
*
* @param manager OffersManager to remove from add to.
* @param mail agent email.
* @param service_name the apartment service name.
* @param apartment_id the apartment id.
*
* @return
*
* 	OFFERS_MANAGER_NULL_PARAMETERS if manager, mail or service_name are NULL
*
* 	OFFERS_MANAGER_OUT_OF_MEMORY in case of memory allocation error
*
* 	OFFERS_MANAGER_SUCCESS the offers removed successfully
*/
OfferManagerResult offersMenagerRemoveAllApartmentOffers(OffersManager manager,
	Email agent_mail, char* service_name, int apartment_id);

/*
* OfferManagerOfferExist: checks if an offer with the given parameters exists
*
* @param manager OffersManager to use.
* @param client Offer's client email.
* @param agent Offer's agent email.
* @param Offer's service_name.
* @param Offer's apartment_id.
*
* @return
* 	false if one of the parameters is NULL or apartment id in negative,
* 	or if an offer was not found; otherwise if an offer found returns true.
*/
bool offersManagerOfferExist(OffersManager manager, Email client,
		Email agent, char* service_name, int apartment_id);

/*
* offersManagerOfferExistForAgent: checks if an offer with the given parameters
* exists, between the agent and a client.
*
* @param manager OffersManager to use.
* @param client Offer's client email.
* @param agent Offer's agent email.
*
* @return
* 	false if one of the parameters is NULL; otherwise if an offer found
* 	returns true.
*/
bool offersManagerOfferExistForAgent(OffersManager manager, Email client,
		Email agent);

/*
* OfferManagerAddOffer: adds an offer with the given parameters
*
* @param manager OffersManager to use.
* @param client Offer's client email.
* @param agent Offer's agent email.
* @param Offer's service_name.
* @param Offer's apartment_id.
* @param Offer's price.
*
* @return
* 	false if one of the parameters is NULL or apartment id in negative,
* 	or if an offer was not found; otherwise if an offer found returns true.
*/
OfferManagerResult offersManagerAddOffer(OffersManager manager,
		Email client_mail, Email agent_mail, char* service_name,
		int apartment_id, int price);

/*
* offersManagerGetOfferDetails: checks if an offer with the given parameters
* exists, between the agent and a client. if one found, saves its details.
*
* @param manager OffersManager to use.
* @param client Offer's client email.
* @param agent Offer's agent email.
*
* @return
* 	false if one of the parameters is NULL; otherwise if an offer found
* 	returns true.
*/
bool offersManagerGetOfferDetails(OffersManager manager, Email client,
		Email agent, int* apartment_id, char** service_name, int* price);

#endif /* SRC_OFFERSMANAGER_H_ */
