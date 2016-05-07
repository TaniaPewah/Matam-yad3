#include <stdlib.h>
#include <stdbool.h>
#include "mtmService.h"
#include "email.h"
#include "clientsManager.h"
#include "agentsManager.h"
#include "offersManager.h"
#include "offer.h"
#include "clientPurchaseBill.h"

#define WALL_CHAR 'w'
#define EMPTY_CHAR 'e'

struct mtmService_t {
	ClientsManager clients;
	AgentsManager agents;
	OffersManager offers;
};

static MTMServiceResult ConvertClientManagerResult(ClientsManagerResult value);
static MTMServiceResult ConvertAgentManagerResult(AgentsManagerResult value);
static MTMServiceResult ConvertEmailResult(EmailResult value);
static MTMServiceResult ConvertOffersManagerResult(OfferManagerResult value);

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
* 	MTM_SERVICE_INVALID_PARAMETERS if service, email_adress or company_name are
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
		char* company_name, int tax_percentage) {
	if ((service == NULL) || (email_adress == NULL) || (company_name == NULL)
		|| (tax_percentage < 1) || (tax_percentage > 100))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult result = emailCreate(email_adress, &mail);
	if (result != EMAIL_SUCCESS) return ConvertEmailResult(result);
	if (clientsManagerClientExists(service->clients, mail) ||
		agentsManagerAgentExists(service->agents, mail)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_ALREADY_EXISTS;
	}
	AgentsManagerResult agents_result = agentsManagerAdd
				(service->agents, mail, company_name, tax_percentage);
	emailDestroy(mail);
	return ConvertAgentManagerResult(agents_result);
}

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
MTMServiceResult mtmServiceRemoveAgent(MTMService service, char* email_adress){
	if ((service == NULL) || (email_adress == NULL))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult result = emailCreate(email_adress, &mail);
	if (result != EMAIL_SUCCESS) return ConvertEmailResult(result);
	bool agent_exists = agentsManagerAgentExists(service->agents, mail);
	bool client_exists = clientsManagerClientExists(service->clients, mail);
	if ((!agent_exists) && (!client_exists)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
	} else if (client_exists) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	AgentsManagerResult agents_result =
		agentsManagerRemove(service->agents, mail);
	if (agents_result != AGENT_MANAGER_SUCCESS) {
		emailDestroy(mail);
		return ConvertAgentManagerResult(agents_result);
	}
	OfferManagerResult offer_result =
		offersMenagerRemoveAllEmailOffers(service->offers, mail);
	emailDestroy(mail);
	return ConvertOffersManagerResult(offer_result);
}

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
		char* email_adress, char* service_name, int max_apartments) {
	if ((service == NULL) || (email_adress == NULL)|| (service_name == NULL) ||
			(max_apartments <= 0)) return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult email_result = emailCreate(email_adress, &mail);
	if (email_result != EMAIL_SUCCESS) return ConvertEmailResult(email_result);
	bool agent_exists = agentsManagerAgentExists(service->agents, mail);
	bool client_exists = clientsManagerClientExists(service->clients, mail);
	if ((!agent_exists) && (!client_exists)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
	} else if (client_exists) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	AgentsManagerResult agents_result = agentsManagerAddApartmentService
			(service->agents, mail, service_name, max_apartments);
	emailDestroy(mail);
	return ConvertAgentManagerResult(agents_result);
}

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
		char* email_adress, char* service_name) {
	if ((service == NULL) || (email_adress == NULL) || (service_name == NULL))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult result = emailCreate(email_adress, &mail);
	if (result != EMAIL_SUCCESS) return ConvertEmailResult(result);
	bool agent_exists = agentsManagerAgentExists(service->agents, mail);
	bool client_exists = clientsManagerClientExists(service->clients, mail);
	if (!agent_exists && !client_exists)
		return MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
	if (client_exists) return MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE;
	AgentsManagerResult agent_result = agentsManagerRemoveApartmentService
			(manager, mail, service_name);
	if (agent_result != CLIENT_MANAGER_SUCCESS) {
		emailDestroy(mail);
		return ConvertClientManagerResult(agent_result);
	}
	OfferManagerResult offer_result = offersMenagerRemoveAllServiceOffers
			(service->offers, mail, service_name);
	emailDestroy(mail);
	return ConvertOffersManagerResult(offer_result);
}

/*
 * mtmServiceAddApartmentToAgent: adds a new apartment to an agent's apartment
 * service.
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
MTMServiceResult mtmServiceAddApartmentToAgent(MTMService service,
		char* email_adress, char* service_name, int id, int price,
		int width, int height, char* matrix) {
	if ((service == NULL) || (email_adress == NULL)|| (service_name == NULL) ||
		(id < 0) || (price <= 0) || ((price % 100) != 0) || (width <= 0) ||
		(height <= 0) || (matrix == NULL) || (strlen(matrix) != height * width)
		|| ((countChar(matrix, EMPTY_CHAR) + countChar(matrix, WALL_CHAR))
				!=  height * width))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult email_result = emailCreate(email_adress, &mail);
	if (email_result != EMAIL_SUCCESS) return ConvertEmailResult(email_result);
	bool agent_exists = agentsManagerAgentExists(service->agents, mail);
	bool client_exists = clientsManagerClientExists(service->clients, mail);
	if ((!agent_exists) && (!client_exists)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
	} else if (client_exists) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	AgentsManagerResult result = agentsManagerAddApartmentToService(
		service->agents, mail, service_name, id, price, width, height, matrix);
	emailDestroy(mail);
	return ConvertAgentManagerResult(result);
}

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
MTMServiceResult mtmServiceAddClient(MTMService service, char* email_adress,
		int min_area, int min_rooms, int max_price) {
	if ((service == NULL) || (email_adress == NULL) || (min_area <= 0)
		|| (min_rooms <= 0) || (max_price <= 0))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult result = emailCreate(email_adress, &mail);
	if (result != EMAIL_SUCCESS) return ConvertEmailResult(result);
	if (clientsManagerClientExists( service->clients, mail) ||
		agentsManagerAgentExists(service->agents, mail)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_ALREADY_EXISTS;
	}
	ClientsManagerResult client_result = clientsManagerAdd
			(service->clients, mail, min_area, min_rooms, max_price);
	emailDestroy(mail);
	return ConvertClientManagerResult(client_result);
}

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
MTMServiceResult mtmServiceRemoveClient(MTMService service,
		char* email_adress) {
	if ((service == NULL) || (email_adress == NULL))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult result = emailCreate(email_adress, &mail);
	if (result != EMAIL_SUCCESS) return ConvertEmailResult(result);
	bool agent_exists = agentsManagerAgentExists(service->agents, mail);
	bool client_exists = clientsManagerClientExists(service->clients, mail);
	if ((!agent_exists) && (!client_exists)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
	} else if (agent_exists) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	ClientsManagerResult client_result =
		clientsManagerRemove(service->clients, mail);
	if (client_result != CLIENT_MANAGER_SUCCESS) {
		emailDestroy(mail);
		return ConvertClientManagerResult(client_result);
	}
	OfferManagerResult offer_result =
		offersMenagerRemoveAllEmailOffers(service->offers, mail);
	emailDestroy(mail);
	return ConvertOffersManagerResult(offer_result);
}





/**
* ConvertClientManagerResult: Converts a ClientsManagerResult to
* MTMServiceResult.
*
* @param value the ClientsManagerResult.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult ConvertClientManagerResult(ClientsManagerResult value){
	MTMServiceResult result;
	switch (value){
	case CLIENT_MANAGER_OUT_OF_MEMORY: {
			result = MTM_SERVICE_OUT_OF_MEMORY;
			break;
		}
	case CLIENT_MANAGER_NULL_PARAMETERS:
	case CLIENT_MANAGER_INVALID_PARAMETERS: {
			result = MTM_SERVICE_INVALID_PARAMETERS;
			break;
		}
	case CLIENT_MANAGER_ALREADY_EXISTS: {
			result = MTM_SERVICE_EMAIL_ALREADY_EXISTS;
			break;
		}
	case CLIENT_MANAGER_NOT_EXISTS: {
			result = MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
			break;
		}
	case CLIENT_MANAGER_SUCCESS:
	default : {
			result = MTM_SERVICE_SUCCESS;
			break;
		}
	}
	return result;
}

/**
* ConvertAgentManagerResult: Converts a AgentsManagerResult to
* MTMServiceResult.
*
* @param value the AgentsManagerResult.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult ConvertAgentManagerResult(AgentsManagerResult value) {
	MTMServiceResult result;
		switch (value){
			case AGENT_MANAGER_OUT_OF_MEMORY: {
				result = MTM_SERVICE_OUT_OF_MEMORY;
				break;
			}
			case AGENT_MANAGER_INVALID_PARAMETERS: {
				result = MTM_SERVICE_INVALID_PARAMETERS;
				break;
			}
			case AGENT_MANAGER_ALREADY_EXISTS: {
				result = MTM_SERVICE_EMAIL_ALREADY_EXISTS;
				break;
			}
			case AGENT_MANAGER_AGENT_NOT_EXISTS: {
				result = MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
				break;
			}
			case AGENT_MANAGER_APARTMENT_NOT_EXISTS: {
				result = MTM_SERVICE_APARTMENT_DOES_NOT_EXIST;
				break;
			}
			case AGENT_MANAGER_SERVICE_NOT_EXISTS: {
				result = MTM_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST;
				break;
			}
			case AGENT_MANAGER_APARTMENT_SERVICE_FULL: {
				result = MTM_SERVICE_APARTMENT_SERVICE_FULL;
				break;
			}
			case AGENT_MANAGER_SUCCESS:
			default : {
				result = MTM_SERVICE_SUCCESS;
				break;
			}
		}
		return result;
}

/**
* ConvertEmailResult: Converts an EmailResult to
* MTMServiceResult.
*
* @param value the EmailResult.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult ConvertEmailResult(EmailResult value) {
	MTMServiceResult result;
	switch (value){
	case EMAIL_OUT_OF_MEMORY: {
			result = MTM_SERVICE_OUT_OF_MEMORY;
			break;
		}
	case EMAIL_NULL_PARAMETERS:
	case EMAIL_INVALID_PARAMETERS: {
			result = MTM_SERVICE_INVALID_PARAMETERS;
			break;
		}
	case EMAIL_SUCCESS:
	default : {
			result = MTM_SERVICE_SUCCESS;
			break;
		}
	}
	return result;
}

/**
* ConvertEmailResult: Converts an OfferManagerResult to
* MTMServiceResult.
*
* @param value the OfferManagerResult.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult ConvertOffersManagerResult(OfferManagerResult value) {
	MTMServiceResult result;
	switch (value){
	case OFFERS_MANAGER_OUT_OF_MEMORY: {
			result = MTM_SERVICE_OUT_OF_MEMORY;
			break;
		}
	case OFFERS_MANAGER_NULL_PARAMETERS:
	case OFFERS_MANAGER_INVALID_PARAMETERS: {
			result = MTM_SERVICE_INVALID_PARAMETERS;
			break;
		}
	case OFFERS_MANAGER_SUCCESS:
	default : {
			result = MTM_SERVICE_SUCCESS;
			break;
		}
	}
	return result;
}
