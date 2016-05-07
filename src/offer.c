#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "email.h"
#include "offer.h"
#include "utilities.h"

struct offer_t {
	Email client;
	Email agent;
	int apartment_id;
	char* service_name;
	int price;
};

/**
* Allocates a new apartment perches offer.
*
* Creates a new offer. This function receives two emails, apartment service
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
						int apartment_id, int price, Offer* result) {
	if (client == NULL || agent == NULL || service_name == NULL ||
			result == NULL) return OFFER_NULL_PARAMETERS;
	if ((price <= 0) || (apartment_id <= 0)) return OFFER_INVALID_PARAMETERS;
	Offer offer = malloc(sizeof(*offer));
	if (offer == NULL) return OFFER_OUT_OF_MEMORY;
	offer->apartment_id = apartment_id;
	offer->price = price;
	if (emailCopy(client, &offer->client) != EMAIL_SUCCESS) {
		free(offer);
		return OFFER_OUT_OF_MEMORY;
	}
	if (emailCopy(agent, &offer->agent) != EMAIL_SUCCESS) {
		emailDestroy(offer->client);
		free(offer);
		return OFFER_OUT_OF_MEMORY;
	}
	offer->service_name = duplicateString(service_name);
	if (offer->service_name == NULL) {
		emailDestroy(offer->agent);
		emailDestroy(offer->client);
		free(offer);
		return OFFER_OUT_OF_MEMORY;
	}
	*result = offer;
	return OFFER_SUCCESS;
}

/**
* offerDestroy: Deallocates an existing offer.
* Clears the offer by using the stored free function.
*
* @param offer Target offer to be deallocated.
*
* If offer is NULL nothing will be done
*/
void offerDestroy(Offer offer) {
	if (offer != NULL) {
		emailDestroy(offer->agent);
		emailDestroy(offer->client);
		free(offer->service_name);
		free(offer);
	}
}

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
bool offerAreOverlapping(Offer first, Offer second) {
	if ((first == NULL) && (first == NULL)) return true;
	if ((first == NULL) || (first == NULL)) return false;
	return ((emailComapre(first->agent, second->agent)) &&
			(emailComapre(first->client, second->client)) &&
			(first->apartment_id == first->apartment_id) &&
			(strcmp(first->service_name, first->service_name) == 0));
}

/**
* offerGetClientEmail: gets the offer client email.
*
* @param offer Target offer.
*
* @return
* 	NULL - if client is NULL
* 	else returns the offer client email.
*/
Email offerGetClientEmail(Offer offer) {
	if (offer == NULL) return NULL;
	return offer->client;
}

/**
* offerGetAgentEmail: gets the offer agent email.
*
* @param offer Target offer.
*
* @return
* 	NULL - if agent is NULL
* 	else returns the offer agent email.
*/
Email offerGetAgentEmail(Offer offer) {
	if (offer == NULL) return NULL;
	return offer->agent;
}

/**
* offerGetServiceName: gets the offer service name.
*
* @param offer Target offer.
*
* @return
* 	NULL - if client is NULL
* 	else returns the offer service name.
*/
char* offerGetServiceName(Offer offer) {
	if (offer == NULL) return NULL;
	return offer->service_name;
}

/**
* offerGetApartmentId: gets the offer apartment id.
*
* @param offer Target offer.
*
* @return
* 	NO_OFFER_VAL - if client is NULL
* 	else returns the offer apartment id.
*/
int offerGetApartmentId(Offer offer) {
	if (offer == NULL) return NO_OFFER_VAL;
	return offer->apartment_id;
}

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
OfferResult offerCopy(Offer offer, Offer* result) {
	if ((offer == NULL) || (result == NULL)) return OFFER_NULL_PARAMETERS;
	return offerCreate(offer->client, offer->agent, offer->service_name,
			offer->apartment_id, offer->price, result);
}

/**
* offerGetPrice: gets the offer suggested price.
*
* @param offer Target offer.
*
* @return
* 	NO_OFFER_VAL - if offer is NULL
* 	else returns the offer suggested price.
*/
int offerGetPrice(Offer offer) {
	if (offer == NULL) return NO_OFFER_VAL;
	return offer->price;
}
