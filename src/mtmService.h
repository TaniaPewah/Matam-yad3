#ifndef SRC_MTMSERVICE_H_
#define SRC_MTMSERVICE_H_

typedef struct mtmService_t *MTMService;

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
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the client removed successfully
*
*/
MTMServiceResult mtmServiceRemoveClient(MTMService service,
		char* email_adress);

#endif /* SRC_MTMSERVICE_H_ */
