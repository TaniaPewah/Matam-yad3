#include <stdlib.h>
#include <stdbool.h>
#include "email.h"
#include "offersManager.h"
#include "offer.h"
#include "list.h"

struct offerManager_t {
	List offers;
};

OfferManagerResult offersMenagerRemoveAllClientOffers(OffersManager manager,
		Email client_mail) {
	return OFFER_MANAGER_SUCCESS;
}

OfferManagerResult offersMenagerRemoveAllAgentOffers(OffersManager manager,
		Email agent_mail) {
	return OFFER_MANAGER_SUCCESS;
}
