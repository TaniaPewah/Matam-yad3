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
	OFFER_MANAGER_INVALID_PARAMETERS = 1,
	OFFER_MANAGER_OUT_OF_MEMORY = 2,
	OFFER_MANAGER_SUCCESS = 3
} OfferManagerResult;


OfferManagerResult offersMenagerRemoveAllClientOffers(OffersManager manager,
		Email client_mail);
OfferManagerResult offersMenagerRemoveAllAgentOffers(OffersManager manager,
		Email agent_mail);

#endif /* SRC_OFFERSMANAGER_H_ */
