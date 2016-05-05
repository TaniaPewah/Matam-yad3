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

