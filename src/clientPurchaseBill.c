#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "clientPurchaseBill.h"
#include "email.h"

struct clientPurchaseBill_t {
	Email email;
	int total_money_paid;
};

/**
* Allocates a new ClientPurchaseBill.
*
* Creates a new ClientPurchaseBill. This function receives the client email,
* and the total money the client paid for apartments and retrives a new
* ClientPurchaseBill.
*
* @param email clients email.
* @param PurchaseBill the total money the client paid for apartments
*
* @return
* 	A new ClientPurchaseBill element or NULL if email is NULL, PurchaseBill is
* 	negative or allocation failed.
*/
ClientPurchaseBill clientPurchaseBillCreate(Email email, int total_money_paid){
	if (email == NULL || total_money_paid < 0) return NULL;
	ClientPurchaseBill bill = malloc(sizeof(*bill));
	if (bill == NULL) return NULL;
	bill->total_money_paid = total_money_paid;
	bill->email = NULL;
	emailCopy(email, &bill->email);
	if (bill->email == NULL) {
		free(bill);
		return NULL;
	}
	return bill;
}

/**
* clientPurchaseBillCopy: Allocates a new ClientPurchaseBill, identical to the
* old ClientPurchaseBill
*
* Creates a new ClientPurchaseBill. This function receives a ClientPurchaseBill,
* and retrieves a new identical ClientPurchaseBill.
*
* @param purchase_bill the ClientPurchaseBill.
*
* @return
* NULL if purchase_bill is NULL or alocation failed; else returns the new
* ClientPurchaseBill.
*/
ClientPurchaseBill clientPurchaseBillCopy(ClientPurchaseBill purchase_bill){
	if (purchase_bill == NULL) return NULL;
	return clientPurchaseBillCreate(purchase_bill->email,
		purchase_bill->total_money_paid);
}

/**
* clientPurchaseBillDestroy: Deallocates an client purchase bill.
* Clears the element by using the stored free function.
*
* @param purchase_bill Target purchase_bill to be deallocated.
*
* If purchase_bill is NULL nothing will be done
*/
void clientPurchaseBillDestroy(ClientPurchaseBill purchase_bill) {
	if (purchase_bill != NULL) {
		emailDestroy(purchase_bill->email);
		free(purchase_bill);
	}
}

/*
 * clientPurchaseBillComapre: Compares two client purchase bills by comparing
 * their total money paid, and if equals their email's.
*
* @param first first clientPurchaseBill.
* @param second second clientPurchaseBill.
*
* @return
*
* 	zero if both are equal or both are NULL.
*
* 	negative value if first paid less than second or first is NULL.
*
* 	positive value if first paid more than second or second is NULL.
*
* 	if both paid the same, retrieves a compare result of the clients emails.
*
*/
int clientPurchaseBillComapre(ClientPurchaseBill first,
							  ClientPurchaseBill second) {
	if ((first == NULL) && (second == NULL)) return 0;
	if (second == NULL) return -1;
	if (first == NULL) return 1;
	int diff = (first->total_money_paid - second->total_money_paid);
	if (diff == 0) {
		diff = emailComapre(first->email, second->email);
	}
	return diff;
}

/**
* clientPurchaseGetMoneyPaidString: gets a string representing the client
* purchase bill, AKA the total sum of money usr spent on buying apartments.
*
* @param bill Target purchase bill.
*
* @return
* 	NULL - if purchase_bill is NULL or in case of memory allocation error,
* 	else a string representing the client purchase bill.
*/
char* clientPurchaseBillGetMoneyPaidString(ClientPurchaseBill bill) {
	if (bill == NULL) return NULL;
	char* result = IntToString(bill->total_money_paid);
	return result;
}

/**
* clientPurchaseGetClientEmailAddress: gets a string representing the client
* email address.
*
* @param bill Target purchase bill.
*
* @return
* 	NULL - if purchase_bill is NULL or in case of memory allocation error,
* 	else a string representing the client email address.
*/
char* clientPurchaseBillGetClientEmailAddress(ClientPurchaseBill bill) {
	if (bill == NULL) return NULL;
	return emailToString(bill->email);
}

