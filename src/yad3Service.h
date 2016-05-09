#ifndef SRC_YAD3SERVICE_H_
#define SRC_YAD3SERVICE_H_

#include "mtm_ex2.h"

typedef struct yad3Service_t *Yad3Service;

#define ACCEPT_STRING accept
#define DECLINE_STRING decline

/**
* This type defines all errors for the system.
* These codes should be used for calling mtmPrintErrorMessage to report
* error messages.
*
* Notice that after calling mtmPrintErrorMessage with one of the first
* three messages you should abort the program according to the exercises
* rules
*/
typedef enum {
	YAD3_SERVICE_OUT_OF_MEMORY,
	YAD3_SERVICE_INVALID_PARAMETERS,
	YAD3_SERVICE_EMAIL_ALREADY_EXISTS,
	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST,
	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE,
	YAD3_SERVICE_ALREADY_REQUESTED,
	YAD3_SERVICE_NOT_REQUESTED,
	YAD3_SERVICE_APARTMENT_SERVICE_ALREADY_EXISTS,
	YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST,
	YAD3_SERVICE_APARTMENT_SERVICE_FULL,
	YAD3_SERVICE_APARTMENT_ALREADY_EXISTS,
	YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST,
	YAD3_SERVICE_PURCHASE_WRONG_PROPERTIES,
	YAD3_SERVICE_REQUEST_WRONG_PROPERTIES,
	YAD3_SERVICE_REQUEST_ILLOGICAL_PRICE,
	YAD3_SERVICE_SUCCESS
} Yad3ServiceResult;

/**
* Allocates a new Yad3Service.
*
* @return
* 	NULL - if allocations failed.
* 	A new service in case of success.
*/
Yad3Service yad3ServiceCreate();

/**
* yad3ServiceDestroy: Deallocates an existing service.
* Clears the elements by using the stored free function.
*
* @param service Target service to be deallocated.
* If service is NULL nothing will be done
*/
void yad3ServiceDestroy(Yad3Service service);

/*
 * yad3ServiceAddAgent: Adds new agent with the given parameters.
*
* @param service service to add to.
* @param email_adress client email address.
* @param company_name the agent's company name. a non empty string.
* @param tax_percentage the agent's tax percentage. A number between 1 to 100.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service, email_adress or company_name
* 		NULL, or if email_adress is illegal, or if tax_percentage is not
* 		between 1 to 100.
*
* 	YAD3_SERVICE_EMAIL_ALREADY_EXISTS if service already contains a client or an
* 		agent under the given email address.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS a new client agent successfully
*
*/
Yad3ServiceResult yad3ServiceAddAgent(Yad3Service service, char* email_adress,
	char* company_name, int tax_percentage);

/*
 * yad3ServiceRemoveAgent: removes agent from service.
*
* @param service service to remove from.
* @param email_adress agent email address.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service or email_adress are NULL,
* 		or if email_adress is illegal.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain an agent with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to a client and not
* 		an agent.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS the agent removed successfully
*
*/
Yad3ServiceResult yad3ServiceRemoveAgent(Yad3Service service, char* email_adress);

/*
 * yad3ServiceAddServiceToAgent: adds a new apartment service to an agent
 * from service.
*
* @param service service of the agents.
* @param email_adress agent email address.
* @param service_name the new service name.
* @param max_apartments maximum number of apartments.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service, service_name or email_adress
* 		are NULL, or if email_adress is illegal.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain an agent with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to a client and not
* 		an agent.
*
* 	MTM_APARTMENT_SERVICE_ALREADY_EXISTS if there is already a service under
* 		that name to the same agent.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS the agent removed successfully
*
*/
Yad3ServiceResult yad3ServiceAddServiceToAgent(Yad3Service service,
	char* email_adress, char* service_name, int max_apartments);

/*
 * yad3ServiceRemoveClient: removes client from service.
*
* @param service service to remove from.
* @param email_adress client email address.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to an agent and not
* 		to a client.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS the client removed successfully
*
*/
Yad3ServiceResult yad3ServiceRemoveServiceFromAgent(Yad3Service service,
	char* email_adress, char* service_name);

/*
 * yad3ServiceAddApartmentToAgent: adds a new apartment to an agent's apartment
 * service.
*
* @param service service of the agents.
* @param email_adress agent email address.
* @param service_name the new service name.
* @param id apartment id.
* @param price apartment price.
* @param width apartment width.
* @param height apartment height.
* @param matrix apartment structure as a string of 'e's and 'w's.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service, service_name or email_adress
* 		are NULL, or if email_adress is illegal.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain an agent with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to a client and not
* 		an agent.
*
* 	YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no service under
* 		that name to the given agent.
*
*	MTM_APARTMENT_SERVICE_FULL The apartment service is full and it is
*		impossible to add a new apartment.
*
* 	YAD3_SERVICE_APARTMENT_ALREADY_EXISTS if there is already an apartment under
* 		the given id.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS the agent removed successfully
*
*/
Yad3ServiceResult yad3ServiceAddApartmentToAgent(Yad3Service service,
	char* email_adress, char* service_name, int id, int price, int width,
	int height, char* matrix);

/*
 * yad3ServiceRemoveApartmentFromAgent: removes an apartment from service.
*
* @param service service to remove from.
* @param email_adress agent email address.
* @param service_name apartment service name.
* @param id apartment id.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to an agent and not
* 		to a client.
*
* 	YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST if the wanted apartment
* 		service does not exist.
*
* 	YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST if the wanted apartment
* 		does not exist.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS the client removed successfully
*
*/
Yad3ServiceResult yad3ServiceRemoveApartmentFromAgent(Yad3Service service,
	char* email_adress, char* service_name, int id);

/*
 * yad3ServiceAddClient: Adds new client with the given parameters.
*
* @param service service to add to.
* @param email_adress client email address.
* @param min_area minimal area for the clients wanted apartments
* @param min_rooms minimal room count in clients wanted apartments
* @param max_price maximum price for the clients wanted apartments
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service or email_adress are NULL,
* 		or if email_adress is illegal, or if min_area or min_rooms or max_price
* 		are negative.
*
* 	YAD3_SERVICE_EMAIL_ALREADY_EXISTS if service already contains a client or an
* 		agent under the given email address.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS a new client added successfully
*
*/
Yad3ServiceResult yad3ServiceAddClient(Yad3Service Service, char* email_adress,
	int minArea, int minRooms, int maxPrice);

/*
 * yad3ServiceRemoveClient: removes client from service.
*
* @param service service to remove from.
* @param email_adress client email address.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE the given email is registered as an
* 		agent and not a client.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS the client removed successfully
*
*/
Yad3ServiceResult yad3ServiceRemoveClient(Yad3Service service,
	char* email_adress);

/*
 * yad3ServiceClientPurchaseApartment: preforms an apartment prochase proccess
 * 	by a client.
*
* @param service service to remove from.
* @param client_email client email address.
* @param agent_email agent email address.
* @param service_name agent apartment service name.
* @param id apartment id.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service, emails, or service_name is NULL,
* 		or if email_adress is illegal, or if id is negative.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE the given email is registered as an
* 		agent and not a client.
*
*	MTM_APARTMENT_SERVICE_DOES_NOT_EXIST if apartment service does not exist
*
*	MTM_APARTMENT_DOES_NOT_EXIST if apartment does not exist
*
*	MTM_PURCHASE_WRONG_PROPERTIES if client cant buy the  apartent, because it
*		is too small for him or if it is too expansive.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	YAD3_SERVICE_SUCCESS the client removed successfully
*
*/
Yad3ServiceResult yad3ServiceClientPurchaseApartment(Yad3Service service,
	char* client_email, char* agent_email, char* service_name,
	int id);

/*
 * CheckOffer: help function that checks if an offer can be purchased by a
 * client.
*
* @param service service to remove from.
* @param client client email.
* @param agent agent email.
* @param service_name agent apartment service name.
* @param id apartment id.
* @param price offer price.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	YAD3_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE the given email is registered as an
* 		agent and not a client.
*
* 	YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST the apartment service does
* 		not exist.
*
* 	YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST the apartment does not exist
*
*	YAD3_SERVICE_ALREADY_REQUESTED if an offer was already suggested
*
*	YAD3_SERVICE_PURCHASE_WRONG_PROPERTIES offer details are wrong
*
*	YAD3_SERVICE_REQUEST_ILLOGICAL_PRICE offer is not good for client
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
*	YAD3_SERVICE_SUCCESS offer can be made
*
*/
Yad3ServiceResult yad3ServiceMakeClientOffer(Yad3Service service,
		char* client_email, char* agent_email, char* service_name, int id,
		int price);

/*
 * yad3ServiceMostPayingCustomers: prints a list with the top most paying
 * customers.
*
* @param service service to print from.
* @param count clients count.
* @param agent agent email.
*
* @return
*
* 	YAD3_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	YAD3_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
*	YAD3_SERVICE_SUCCESS offer can be made
*
*/
Yad3ServiceResult yad3ServiceMostPayingCustomers(Yad3Service service,
		int count, FILE* output);





#endif /* SRC_YAD3SERVICE_H_ */
