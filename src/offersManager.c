#include <stdlib.h>
#include <stdbool.h>
#include "email.h"
#include "offersManager.h"
#include "offer.h"
#include "list.h"
#include "utilities.h"

struct offerManager_t {
	List offers;
};

/**
* This type defines end codes for the compare methods.
*/
typedef enum {
	COMPARE_OUT_OF_MEMORY = 0,
	COMPARE_FIT = 1,
	COMPARE_UNFIT = 2
} CompareResult;

typedef void* checkOfferParam;
typedef CompareResult (*checkOffer)(Offer offer, checkOfferParam param);

static OfferManagerResult filteredRemoveOffers(OffersManager manager,
	checkOffer function, checkOfferParam param);
static CompareResult isEmailConnectedToOffer(Offer offer,
	checkOfferParam parameter);
static CompareResult isApartmentConnectedToOffer(Offer offer,
	checkOfferParam parameter);

static void FreeOfferListElement(ListElement element);
static ListElement CopyOfferListElement(ListElement);

/**
* Allocates a new OfferManager.
*
* @return
* 	NULL - if allocations failed.
* 	A new clients in case of success.
*/
OffersManager OfferManagerCreate() {
	List list = listCreate(CopyOfferListElement, FreeOfferListElement);
	if (list == NULL) return NULL;
	OffersManager manager = malloc (sizeof(*manager));
	if (manager == NULL) {
		free(list);
		return NULL;
	} else {
		manager->offers = list;
		return manager;
	}
}

/** Function to be used for coping offer data elements from the list */
static ListElement CopyOfferListElement(ListElement element) {
	if (element == NULL) return NULL;
	Offer new_offer = NULL;
	offerCopy((Offer)element, &new_offer);
	return new_offer;
}

/** Function to be used for freeing offer data elements from the list */
static void FreeOfferListElement(ListElement element) {
	if (element != NULL) {
		offerDestroy((Offer)element);
	}
}

/**
* offerManagerDestroy: Deallocates an existing offers manager.
* Clears the element by using the stored free function.
*
* @param offer Target offer to be deallocated.
* If offer is NULL nothing will be done
*/
void offersManagerDestroy(OffersManager offer) {
	if (offer != NULL) {
		listDestroy(offer->offers);
		free(offer);
	}
}

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
	Email mail) {
	return filteredRemoveOffers(manager, isEmailConnectedToOffer, mail);
}

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
	Email mail, char* service_name, int apartment_id) {
	if ((manager == NULL) || (mail == NULL) || (service_name == NULL))
		return OFFERS_MANAGER_NULL_PARAMETERS;
	void** parameters = malloc (3 * sizeof(void*));
	if (parameters == NULL) return OFFERS_MANAGER_OUT_OF_MEMORY;
	parameters[0] = mail;
	parameters[1] = service_name;
	parameters[2] = &apartment_id;
	OfferManagerResult result = filteredRemoveOffers(manager,
			isApartmentConnectedToOffer, parameters);
	free (parameters);
	return result;
}

/*
* offersManagerRemoveOffer: removes all the offers from the given offer manager
* that are associated to the given parameters. the association is checked
* using the given checkOffer method and uses the given checkOfferParam
* parameters.
*
* For example, here is a proper function for checking if an offer's client
* has the given email address:
*
* @code
* CompareResult compareOffersEquel(Offer first, checkOfferParam parameter) {
*   if (parameter == NULL) return COMPARE_UNFIT;
*   return emailAreEqual(offerGetClientEmail(offer), (Email)parameter) ?
*   	COMPARE_FIT : COMPARE_UNFIT;
* }
*
* @endcode
*
* @param offer OffersManager to remove from add to.
* @param checkOfferParam parameters to check.
*
* @return
*
* 	OFFERS_MANAGER_NULL_PARAMETERS if manager or function are NULL.
*
* 	OFFERS_MANAGER_SUCCESS associated offers had been removed successfully.
*
* 	OFFERS_MANAGER_OUT_OF_MEMORY method had a memory allocation error.
*/
static OfferManagerResult filteredRemoveOffers(OffersManager manager,
		checkOffer function, checkOfferParam param) {
	if ((manager == NULL) || (param == NULL))
		return OFFERS_MANAGER_NULL_PARAMETERS;
	Offer current = (Offer)listGetFirst(manager->offers);
	while (current != NULL) {
		CompareResult copare = function(current, param);
		if (copare == COMPARE_FIT) {
			listRemoveCurrent(manager->offers);
			// TODO : What about this ? is it needed???
			current = (Offer)listGetFirst(manager->offers);
		} else if (copare == COMPARE_UNFIT) {
			current = listGetNext(manager->offers);
		} else {
			return OFFERS_MANAGER_OUT_OF_MEMORY;
		}
	}
	return OFFERS_MANAGER_SUCCESS;
}

/*
* isEmailConnectedToOffer: checks if the offers is associated with the given
* email string adress.
*
* @param offer offer to compare.
* @param parameter a pointer to the email string address.
*
* @return
*
* 	COMPARE_UNFIT if manager or parameter NULL, or if the email address is
* 		invalid, or if the email is not associated with the offer.
*
* 	COMPARE_FIT email is associated with the offer.
*
* 	COMPARE_OUT_OF_MEMORY method had a memory allocation error.
*/
static CompareResult isEmailConnectedToOffer(Offer offer,
		checkOfferParam parameter) {
	if ((offer == NULL) || (parameter == NULL)) return COMPARE_UNFIT;
	Email mail;
	EmailResult result = emailCreate((char*)parameter, &mail);
	if (result == EMAIL_OUT_OF_MEMORY) return COMPARE_OUT_OF_MEMORY;
	if (result == EMAIL_NULL_PARAMETERS || result == EMAIL_INVALID_PARAMETERS)
		return COMPARE_UNFIT;
	bool fits = ((emailAreEqual(offerGetClientEmail(offer), mail)) ||
			     (emailAreEqual(offerGetClientEmail(offer), mail)));
	emailDestroy(mail);
	return fits ? COMPARE_FIT : COMPARE_UNFIT;
}

/*
* isApartmentConnectedToOffer: checks if the offers is associated with the
* given apartment identifier.
*
* @param offer Offer to check.
* @param parameter a pointer to a three cells array. first cell contains the
* 	agent Email, the second contains the service name and the last one contains
* 	a pointer to the apartment id.
*
* @return
*
* 	COMPARE_UNFIT if manager or parameter NULL, or if the email address is
* 		invalid, or if the email is not associated with the offer.
*
* 	COMPARE_FIT email is associated with the offer.
*
* 	COMPARE_OUT_OF_MEMORY method had a memory allocation error.
*/
static CompareResult isApartmentConnectedToOffer(Offer offer,
		checkOfferParam parameter) {
	if ((offer == NULL) || (parameter == NULL)) return COMPARE_UNFIT;
	return ((emailAreEqual(offerGetAgentEmail(offer),
				(Email)(((void**)parameter)[0]))) &&
			(areStringsEqual(offerGetServiceName(offer),
			    (char*)(((void**)parameter)[1]))) &&
			(offerGetApartmentId(offer) == *(int*)(((void**)parameter)[2]))) ?
			COMPARE_FIT : COMPARE_UNFIT;
}
