#ifndef SRC_MTMSERVICE_H_
#define SRC_MTMSERVICE_H_

typedef struct mtmService_t *MTMService;

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
	MTM_SERVICE_OUT_OF_MEMORY,
	MTM_SERVICE_INVALID_PARAMETERS,
	MTM_SERVICE_EMAIL_ALREADY_EXISTS,
	MTM_SERVICE_EMAIL_DOES_NOT_EXIST,
	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE,
	MTM_SERVICE_ALREADY_REQUESTED,
	MTM_SERVICE_NOT_REQUESTED,
	MTM_SERVICE_APARTMENT_SERVICE_ALREADY_EXISTS,
	MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST,
	MTM_SERVICE_APARTMENT_SERVICE_FULL,
	MTM_SERVICE_APARTMENT_ALREADY_EXISTS,
	MTM_SERVICE_APARTMENT_DOES_NOT_EXIST,
	MTM_SERVICE_PURCHASE_WRONG_PROPERTIES,
	MTM_SERVICE_REQUEST_WRONG_PROPERTIES,
	MTM_SERVICE_REQUEST_ILLOGICAL_PRICE,
	MTM_SERVICE_SUCCESS
} MTMServiceResult;

/**
* Allocates a new MTMService.
*
* @return
* 	NULL - if allocations failed.
* 	A new service in case of success.
*/
MTMService mtmServiceCreate();

/**
* mtmServiceDestroy: Deallocates an existing service.
* Clears the elements by using the stored free function.
*
* @param service Target service to be deallocated.
* If service is NULL nothing will be done
*/
void mtmServiceDestroy(MTMService service);

/*
 * mtmServiceAddAgent: Adds new agent with the given parameters.
*
* @param service service to add to.
* @param email_adress client email address.
* @param company_name the agent's company name. a non empty string.
* @param tax_percentage the agent's tax percentage. A number between 1 to 100.
*
* @return
*
* 	MTM_SERVICE_INVALID_PARAMETERS if service, email_adress or company_name
* 		NULL, or if email_adress is illegal, or if tax_percentage is not
* 		between 1 to 100.
*
* 	MTM_SERVICE_EMAIL_ALREADY_EXISTS if service already contains a client or an
* 		agent under the given email address.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS a new client agent successfully
*
*/
MTMServiceResult mtmServiceAddAgent(MTMService service, char* email_adress,
	char* company_name, int tax_percentage);

/*
 * mtmServiceRemoveAgent: removes agent from service.
*
* @param service service to remove from.
* @param email_adress agent email address.
*
* @return
*
* 	MTM_SERVICE_INVALID_PARAMETERS if service or email_adress are NULL,
* 		or if email_adress is illegal.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain an agent with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to a client and not
* 		an agent.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the agent removed successfully
*
*/
MTMServiceResult mtmServiceRemoveAgent(MTMService service, char* email_adress);

/*
 * mtmServiceAddServiceToAgent: adds a new apartment service to an agent
 * from service.
*
* @param service service of the agents.
* @param email_adress agent email address.
* @param service_name the new service name.
* @param max_apartments maximum number of apartments.
*
* @return
*
* 	MTM_SERVICE_INVALID_PARAMETERS if service, service_name or email_adress
* 		are NULL, or if email_adress is illegal.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain an agent with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to a client and not
* 		an agent.
*
* 	MTM_APARTMENT_SERVICE_ALREADY_EXISTS if there is already a service under
* 		that name to the same agent.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the agent removed successfully
*
*/
MTMServiceResult mtmServiceAddServiceToAgent(MTMService service,
	char* email_adress, char* service_name, int max_apartments);

/*
 * mtmServiceRemoveClient: removes client from service.
*
* @param service service to remove from.
* @param email_adress client email address.
*
* @return
*
* 	MTM_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to an agent and not
* 		to a client.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the client removed successfully
*
*/
MTMServiceResult mtmServiceRemoveServiceFromAgent(MTMService service,
	char* email_adress, char* service_name);

/*
 * mtmServiceAddApartmentToAgent: adds a new apartment to an agent's apartment
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
* 	MTM_SERVICE_INVALID_PARAMETERS if service, service_name or email_adress
* 		are NULL, or if email_adress is illegal.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain an agent with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to a client and not
* 		an agent.
*
* 	MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST if there is no service under
* 		that name to the given agent.
*
*	MTM_APARTMENT_SERVICE_FULL The apartment service is full and it is
*		impossible to add a new apartment.
*
* 	MTM_SERVICE_APARTMENT_ALREADY_EXISTS if there is already an apartment under
* 		the given id.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the agent removed successfully
*
*/
MTMServiceResult mtmServiceAddApartmentToAgent(MTMService service,
	char* email_adress, char* service_name, int id, int price, int width,
	int height, char* matrix);

/*
 * mtmServiceRemoveApartmentFromAgent: removes an apartment from service.
*
* @param service service to remove from.
* @param email_adress agent email address.
* @param service_name apartment service name.
* @param id apartment id.
*
* @return
*
* 	MTM_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE is email belongs to an agent and not
* 		to a client.
*
* 	MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST if the wanted apartment
* 		service does not exist.
*
* 	MTM_SERVICE_APARTMENT_DOES_NOT_EXIST if the wanted apartment
* 		does not exist.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the client removed successfully
*
*/
MTMServiceResult mtmServiceRemoveApartmentFromAgent(MTMService service,
	char* email_adress, char* service_name, int id);

/*
 * mtmServiceAddClient: Adds new client with the given parameters.
*
* @param service service to add to.
* @param email_adress client email address.
* @param min_area minimal area for the clients wanted apartments
* @param min_rooms minimal room count in clients wanted apartments
* @param max_price maximum price for the clients wanted apartments
*
* @return
*
* 	MTM_SERVICE_INVALID_PARAMETERS if service or email_adress are NULL,
* 		or if email_adress is illegal, or if min_area or min_rooms or max_price
* 		are negative.
*
* 	MTM_SERVICE_EMAIL_ALREADY_EXISTS if service already contains a client or an
* 		agent under the given email address.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS a new client added successfully
*
*/
MTMServiceResult mtmServiceAddClient(MTMService Service, char* email_adress,
	int minArea, int minRooms, int maxPrice);

/*
 * mtmServiceRemoveClient: removes client from service.
*
* @param service service to remove from.
* @param email_adress client email address.
*
* @return
*
* 	MTM_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE the given email is registered as an
* 		agent and not a client.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the client removed successfully
*
*/
MTMServiceResult mtmServiceRemoveClient(MTMService service,
	char* email_adress);

/*
 * mtmServiceClientPurchaseApartment: preforms an apartment prochase proccess
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
* 	MTM_SERVICE_INVALID_PARAMETERS if service, emails, or service_name is NULL,
* 		or if email_adress is illegal, or if id is negative.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE the given email is registered as an
* 		agent and not a client.
*
*	MTM_APARTMENT_SERVICE_DOES_NOT_EXIST if apartment service does not exist
*
*	MTM_APARTMENT_DOES_NOT_EXIST if apartment does not exist
*
*	MTM_PURCHASE_WRONG_PROPERTIES if client cant buy the  apartent, because it
*		is too small for him or if it is too expansive.
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the client removed successfully
*
*/
MTMServiceResult mtmServiceClientPurchaseApartment(MTMService service,
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
* 	MTM_SERVICE_INVALID_PARAMETERS if service or are email_adress NULL,
* 		or if email_adress is illegal.
*
* 	MTM_SERVICE_EMAIL_DOES_NOT_EXIST if service does not contain a client with
* 		the given email address.
*
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE the given email is registered as an
* 		agent and not a client.
*
* 	MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST the apartment service does
* 		not exist.
*
* 	MTM_SERVICE_APARTMENT_DOES_NOT_EXIST the apartment does not exist
*
*	MTM_SERVICE_ALREADY_REQUESTED if an offer was already suggested
*
*	MTM_SERVICE_PURCHASE_WRONG_PROPERTIES offer details are wrong
*
*	MTM_SERVICE_REQUEST_ILLOGICAL_PRICE offer is not good for client
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
*	MTM_SERVICE_SUCCESS offer can be made
*
*/
MTMServiceResult mtmServiceMakeClientOffer(MTMService service,
		char* client_email, char* agent_email, char* service_name, int id,
		int price);

#endif /* SRC_MTMSERVICE_H_ */
