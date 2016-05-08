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
OffersManager OfferManagerCreate();

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

#endif /* SRC_OFFERSMANAGER_H_ */
