#ifndef SRC_OFFER_H_
#define SRC_OFFER_H_

#include "email.h"

#define NO_OFFER_VAL -1

typedef struct offer_t *Offer;

/**
* This type defines end codes for the methods.
*/
typedef enum {
	OFFER_NULL_PARAMETERS = 0,
	OFFER_INVALID_PARAMETERS = 1,
	OFFER_OUT_OF_MEMORY = 2,
	OFFER_SUCCESS = 3
} OfferResult;

/**
* Allocates a new apartment perches offer.
*
* Creates a new offer. This function receives two Emails, apartment service
* name, apartment id and price, and creates an element.
*
* @param address the email address.
* @param result pointer to save the result email in
*
* @return
*
* 	OFFER_NULL_PARAMETERS - if emails or service_name or result are NULL.
*
* 	OFFER_INVALID_PARAMETERS - apartment_id or price are not positive.
*
* 	OFFER_OUT_OF_MEMORY - if allocations failed.
*
* 	OFFER_SUCCESS - in case of success.  A new offer is saved in the result.
*/
OfferResult offerCreate(Email client, Email agent, char* service_name,
						int apartment_id, int price, Offer* result);

/**
* offerDestroy: Deallocates an existing offer.
* Clears the offer by using the stored free function.
*
* @param offer Target offer to be deallocated.
*
* If offer is NULL nothing will be done
*/
void offerDestroy(Offer offer);

/*
 * offerAreOverlapping: Checks if both offer are overlapping, means that have
 * the same costumer, agent, apartment id and service name
*
* @param first first offer.
* @param second second offer.
*
* @return
* 	true if both are NULL or overlapping, else returns false.
*/
bool offerAreOverlapping(Offer first, Offer second);

/**
* offerGetClientEmail: gets the offer client email.
*
* @param offer Target offer.
*
* @return
* 	NULL - if client is NULL
* 	else returns the offer client email.
*/
Email offerGetClientEmail(Offer offer);

/**
* offerGetAgentEmail: gets the offer agent email.
*
* @param offer Target offer.
*
* @return
* 	NULL - if agent is NULL
* 	else returns the offer agent email.
*/
Email offerGetAgentEmail(Offer offer);

/**
* offerGetServiceName: gets the offer service name.
*
* @param offer Target offer.
*
* @return
* 	NULL - if client is NULL
* 	else returns the offer service name.
*/
char* offerGetServiceName(Offer offer);

/**
* offerGetApartmentId: gets the offer apartment id.
*
* @param offer Target offer.
*
* @return
* 	NO_OFFER_VAL - if client is NULL
* 	else returns the offer apartment id.
*/
int offerGetApartmentId(Offer offer);

/**
* Allocates a new offer, identical to the old offer
*
* Creates a new offer. This function receives an offer elemnet, and retrieves
* a new identical offer element pointer in the out pointer parameter.
*
* @param offer the original offer.
* @param result pointer to save the new offer in.
*
* @return
*
* 	OFFER_NULL_PARAMETERS - if offer or pointer are NULL.

* 	Offer_OUT_OF_MEMORY - if allocations failed.
*
* 	EMAIL_SUCCESS - in case of success. A new offer is saved in the result.
*/
OfferResult offerCopy(Offer offer, Offer* result);

#endif /* SRC_OFFER_H_ */
