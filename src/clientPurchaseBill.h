#ifndef SRC_CLIENTPurchaseBill_H_
#define SRC_CLIENTPurchaseBill_H_

#include "email.h"

typedef struct clientPurchaseBill_t *ClientPurchaseBill;

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
ClientPurchaseBill clientPurchaseBillCreate(Email email, int total_money_paid);

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
ClientPurchaseBill clientPurchaseBillCopy(ClientPurchaseBill purchase_bill);

/**
* clientPurchaseBillDestroy: Deallocates an client purchase bill.
* Clears the element by using the stored free function.
*
* @param purchase_bill Target purchase_bill to be deallocated.
*
* If purchase_bill is NULL nothing will be done
*/
void clientPurchaseBillDestroy(ClientPurchaseBill purchase_bill);

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
* 	negative value if first is less than second or first is NULL.
*
* 	positive value if first is less than second or second is NULL.
*/
int clientPurchaseBillComapre(ClientPurchaseBill first,
							  ClientPurchaseBill second);

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
char* clientPurchaseBillGetMoneyPaidString(ClientPurchaseBill bill);

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
char* clientPurchaseBillGetClientEmailAddress(ClientPurchaseBill bill);

#endif /* SRC_CLIENTPurchaseBill_H_ */
