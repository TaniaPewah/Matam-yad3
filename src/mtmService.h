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
} ServiceResult;

#endif /* SRC_MTMSERVICE_H_ */
