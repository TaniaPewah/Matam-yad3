#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mtmService.h"
#include "email.h"
#include "utilities.h"
#include "clientsManager.h"
#include "agentsManager.h"
#include "offersManager.h"
#include "clientPurchaseBill.h"

#define WALL_CHAR 'w'
#define EMPTY_CHAR 'e'

struct mtmService_t {
	ClientsManager clients;
	AgentsManager agents;
	OffersManager offers;
};

static MTMServiceResult RemoveApartmentFromAgent(MTMService service, Email mail,
	char* service_name, int id);
static MTMServiceResult CreateEmailAndSearchForClient(MTMService service,
	char* email_adress, Email *out_email);
static MTMServiceResult CreateEmailAndSearchForAgent(MTMService service,
	char* email_adress, Email *out_email);
static MTMServiceResult CreateEmailAndSearch(MTMService service,
	char* email_adress, Email *out_email, bool search_for_client);
static MTMServiceResult convertClientManagerResult(ClientsManagerResult value);
static MTMServiceResult ConvertAgentManagerResult(AgentsManagerResult value);
static MTMServiceResult convertEmailResult(EmailResult value);
static MTMServiceResult convertOffersManagerResult(OfferManagerResult value);
static MTMServiceResult CheckClientPurchaseApartment(MTMService service,
	Email client, Email agent, char* service_name, int id);
static MTMServiceResult CommitClientPurchaseApartment(MTMService service,
	Email client, Email agent, char* service_name, int id, int finalPrice);
static MTMServiceResult CheckOffer(MTMService service, Email client,
	Email agent, char* service_name, int id, int price);

/**
* Allocates a new MTMService.
*
* @return
* 	NULL - if allocations failed.
* 	A new service in case of success.
*/
MTMService mtmServiceCreate() {
	ClientsManager client_manager = clientsManagerCreate();
	if (client_manager == NULL) return NULL;
	AgentsManager agent_manager = agentsManagerCreate();
	if (agent_manager == NULL) {
		clientsManagerDestroy(client_manager);
		return NULL;
	}
	OffersManager offer_manager = offerManagerCreate();
	if (offer_manager == NULL) {
		clientsManagerDestroy(client_manager);
		agentsManagerDestroy(agent_manager);
		return NULL;
	}
	MTMService service = malloc(sizeof(*service));
	if (service == NULL) {
			clientsManagerDestroy(client_manager);
			agentsManagerDestroy(agent_manager);
			offerManagerDestroy(offer_manager);
			return NULL;
		}
	service->clients = client_manager;
	service->agents = agent_manager;
	service->offers = offer_manager;
	return service;
}

/**
* mtmServiceDestroy: Deallocates an existing service.
* Clears the elements by using the stored free function.
*
* @param service Target service to be deallocated.
* If service is NULL nothing will be done
*/
void mtmServiceDestroy(MTMService service) {
	if (service != NULL) {
			clientsManagerDestroy(service->clients);
			agentsManagerDestroy(service->agents);
			offerManagerDestroy(service->offers);
		}
}

/*
 *
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
	if (result != EMAIL_SUCCESS) return convertEmailResult(result);
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
	MTMServiceResult result = CreateEmailAndSearchForAgent(service,
			email_adress, &mail);
	if (result != MTM_SERVICE_SUCCESS)	return result;
	AgentsManagerResult agents_result =
		agentsManagerRemove(service->agents, mail);
	if (agents_result != AGENT_MANAGER_SUCCESS) {
		emailDestroy(mail);
		return ConvertAgentManagerResult(agents_result);
	}
	OfferManagerResult offer_result =
		offersMenagerRemoveAllEmailOffers(service->offers, mail);
	emailDestroy(mail);
	return convertOffersManagerResult(offer_result);
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
	MTMServiceResult result = CreateEmailAndSearchForAgent(service,
		email_adress, &mail);
	if (result != MTM_SERVICE_SUCCESS) return result;
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
	MTMServiceResult result = CreateEmailAndSearchForAgent(service,
		email_adress, &mail);
	if (result != MTM_SERVICE_SUCCESS) return result;
	AgentsManagerResult agent_result = agentsManagerRemoveApartmentService
			(service->agents, mail, service_name);
	if (agent_result != AGENT_MANAGER_SUCCESS) {
		emailDestroy(mail);
		return convertClientManagerResult(agent_result);
	}
	OfferManagerResult offer_result = offersMenagerRemoveAllServiceOffers
			(service->offers, mail, service_name);
	emailDestroy(mail);
	return convertOffersManagerResult(offer_result);
}

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
		char* email_adress, char* service_name, int id, int price,
		int width, int height, char* matrix) {
	if ((service == NULL) || (email_adress == NULL)|| (service_name == NULL) ||
		(id < 0) || (price <= 0) || ((price % 100) != 0) || (width <= 0) ||
		(height <= 0) || (matrix == NULL) || (strlen(matrix) != height * width)
		|| ((countChar(matrix, EMPTY_CHAR) + countChar(matrix, WALL_CHAR))
				!=  height * width)) return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	EmailResult email_result = emailCreate(email_adress, &mail);
	if (email_result != EMAIL_SUCCESS) return convertEmailResult(email_result);
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
	char* email_adress, char* service_name, int id) {
	if ((service == NULL) || (email_adress == NULL) || (service_name == NULL))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email mail = NULL;
	MTMServiceResult search_result = CreateEmailAndSearchForAgent(service,
		email_adress, &mail);
	if (search_result != MTM_SERVICE_SUCCESS) return search_result;
	MTMServiceResult result = RemoveApartmentFromAgent(service,
		mail, service_name, id);
	emailDestroy(mail);
	return result;
}

/*
 * RemoveApartmentFromAgent: help function that removes an apartment from
 * an agents service.
*
* @param service service to remove from.
* @param mail agent email.
* @param service_name apartment service name.
* @param id apartment id.
*
* @return
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
static MTMServiceResult RemoveApartmentFromAgent(MTMService service,
		Email mail, char* service_name, int id) {
	AgentsManagerResult agent_result = agentsManagerRemoveApartmentFromService
		(service->agents, mail, service_name, id);
	if (agent_result != AGENT_MANAGER_SUCCESS) {
		return ConvertAgentManagerResult(agent_result);
	}
	OfferManagerResult offer_result = offersMenagerRemoveAllApartmentOffers
		(service->offers, mail, service_name, id);
	return convertOffersManagerResult(offer_result);
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
	if (result != EMAIL_SUCCESS) return convertEmailResult(result);
	if (clientsManagerClientExists( service->clients, mail) ||
		agentsManagerAgentExists(service->agents, mail)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_ALREADY_EXISTS;
	}
	ClientsManagerResult client_result = clientsManagerAdd
			(service->clients, mail, min_area, min_rooms, max_price);
	emailDestroy(mail);
	return convertClientManagerResult(client_result);
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
* 	MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE the given email is registered as an
* 		agent and not a client.
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
	MTMServiceResult result = CreateEmailAndSearchForClient(service,
		email_adress, &mail);
	if (result != MTM_SERVICE_SUCCESS) return result;
	ClientsManagerResult client_result =
		clientsManagerRemove(service->clients, mail);
	if (client_result != CLIENT_MANAGER_SUCCESS) {
		emailDestroy(mail);
		return convertClientManagerResult(client_result);
	}
	OfferManagerResult offer_result =
		offersMenagerRemoveAllEmailOffers(service->offers, mail);
	emailDestroy(mail);
	return convertOffersManagerResult(offer_result);
}

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
		int price) {
	if ((service == NULL) || (client_email == NULL) || (agent_email == NULL)
			|| (id < 0) || (price <=0)) return MTM_SERVICE_INVALID_PARAMETERS;
	Email client = NULL, agent = NULL;
	MTMServiceResult client_result = CreateEmailAndSearchForClient(service,
		client_email, &client);
	if (client_result != MTM_SERVICE_SUCCESS) return client_result;
	MTMServiceResult agent_result = CreateEmailAndSearchForAgent(service,
		client_email, &agent);
	if (agent_result != MTM_SERVICE_SUCCESS) return agent_result;
	MTMServiceResult offer_check_result =
			CheckOffer(service, client, agent, service_name, id, price);
	if(offer_check_result != MTM_SERVICE_SUCCESS) {
		emailDestroy(client);
		emailDestroy(agent);
		return offer_check_result;
	}
	OfferManagerResult add_result = OfferManagerAddOffer(service->offers,
			client, agent, service_name, id, price);
	emailDestroy(client);
	emailDestroy(agent);
	return convertOffersManagerResult(add_result);
}

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
static MTMServiceResult CheckOffer(MTMService service, Email client,
	Email agent, char* service_name, int id, int price) {
	if (OfferManagerOfferExist(service->offers, client, agent,
			service_name, id)) return MTM_SERVICE_ALREADY_REQUESTED;
	int apartment_area, apartment_rooms, apartment_price, apartment_commition;
	AgentsManagerResult aprtment_result = agentsManagerGetApartmentDetails(
		service->agents, agent, service_name, id, &apartment_area,
		&apartment_rooms, &apartment_price, &apartment_commition);
	if (aprtment_result != AGENT_MANAGER_SUCCESS)
		return ConvertAgentManagerResult(aprtment_result);
	int client_min_area, client_min_room, client_max_price;
	ClientsManagerResult restriction_result = clientsManagerGetRestriction(
		service->clients, client, &client_min_area, &client_min_room,
		&client_max_price);
	if (restriction_result != CLIENT_MANAGER_SUCCESS)
		return ConvertAgentManagerResult(aprtment_result);
	if ((apartment_rooms < client_min_room) ||
		(apartment_area < client_min_area) ||
		(client_max_price < price))
		return MTM_SERVICE_PURCHASE_WRONG_PROPERTIES;
	if (apartment_price * (100 + apartment_commition) < price)
		return MTM_SERVICE_REQUEST_ILLOGICAL_PRICE;
	return MTM_SERVICE_SUCCESS;
}

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
	int id) {
	if ((service == NULL) || (client_email == NULL) || (id < 0) ||
		(agent_email == NULL) || (service_name == NULL))
		return MTM_SERVICE_INVALID_PARAMETERS;
	Email client = NULL, agent = NULL;
	MTMServiceResult search_result = CreateEmailAndSearchForClient(service,
			client_email, &client);
	if (search_result != MTM_SERVICE_SUCCESS) return search_result;
	search_result = CreateEmailAndSearchForAgent(service, client_email,
		&client);
	if (search_result != MTM_SERVICE_SUCCESS) {
		emailDestroy(client);
		return search_result;
	}
	MTMServiceResult final = CheckClientPurchaseApartment
			(service, client, agent, service_name, id);
	emailDestroy(client);
	emailDestroy(agent);
	return final;
}

/*
 * CheckClientPurchaseApartment: help function that checks if an apartment can
 *  be prochased by a client. if so, calls the CommitClientPurchaseApartment
 *  function.
*
* @param service service to remove from.
* @param client client email.
* @param agent agent email.
* @param service_name agent apartment service name.
* @param id apartment id.
*
* @return
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
static MTMServiceResult CheckClientPurchaseApartment(MTMService service,
	Email client, Email agent, char* service_name, int id) {
	int apartment_area, apartment_rooms, apartment_price, apartment_commition;
		AgentsManagerResult aprtment_result = agentsManagerGetApartmentDetails(
			service->agents, agent, service_name, id, &apartment_area,
			&apartment_rooms, &apartment_price, &apartment_commition);
	if (aprtment_result != AGENT_MANAGER_SUCCESS)
		return ConvertAgentManagerResult(aprtment_result);
	int client_min_area, client_min_room, client_max_price;
	ClientsManagerResult client_result = clientsManagerGetRestriction(
		service->clients, client, &client_min_area, &client_min_room,
		&client_max_price);
	if (client_result != CLIENT_MANAGER_SUCCESS)
		return ConvertAgentManagerResult(aprtment_result);
	if ((apartment_rooms < client_min_room) ||
		(apartment_area < client_min_area) ||
		(client_max_price < apartment_price * (100 + apartment_commition)))
		return MTM_SERVICE_PURCHASE_WRONG_PROPERTIES;
	return CommitClientPurchaseApartment(service,client, agent,
		service_name, id, apartment_price * (100 + apartment_commition));
}

/*
 * CheckClientPurchaseApartment: help function that commits the actual
 * apartment prochase by a client process.
 * uses the RemoveApartmentFromAgent method to remve the apartment and offers
 * reated to the apartment.
*
* @param service service to remove from.
* @param client client email.
* @param agent agent email.
* @param service_name agent apartment service name.
* @param id apartment id.
*
* @return
*
* 	MTM_SERVICE_OUT_OF_MEMORY in case of memory allocation problem.
*
* 	MTM_SERVICE_SUCCESS the client removed successfully
*
*/
static MTMServiceResult CommitClientPurchaseApartment(MTMService service,
	Email client, Email agent, char* service_name, int id, int finalPrice) {
	ClientsManagerResult result = clientsManagerExecurePurchase
			(service->clients, client, finalPrice);
	if (result != CLIENT_MANAGER_SUCCESS)
		return convertClientManagerResult(result);
	return RemoveApartmentFromAgent(service, agent, service_name, id);
}

/**
* CreateEmailAndSearchForClient: Creates an email and checks if it is a
* 	registered client.
*
* @param service service to check.
* @param email_adress string representing the email.
* @param out_email Created email pointer to save to.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult CreateEmailAndSearchForClient(MTMService service,
		char* email_adress, Email *out_email) {
	return (CreateEmailAndSearch(service, email_adress, out_email, true));
}

/**
* CreateEmailAndSearchForAgent: Creates an email and checks if it is a
* 	registered agent.
*
* @param service service to check.
* @param email_adress string representing the email.
* @param out_email Created email pointer to save to.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult CreateEmailAndSearchForAgent(MTMService service,
		char* email_adress, Email *out_email) {
	return (CreateEmailAndSearch(service, email_adress, out_email, false));
}

/**
* CreateEmailAndSearch: Creates an email and checks if it is a
* 	registered client or agent.
*
* @param service service to check.
* @param email_adress string representing the email.
* @param out_email Created email pointer to save to.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult CreateEmailAndSearch(MTMService service,
		char* email_adress, Email *out_email, bool search_for_client) {
	Email mail = NULL;
	EmailResult result = emailCreate(email_adress, &mail);
	if (result != EMAIL_SUCCESS) return convertEmailResult(result);
	*out_email = mail;
	bool agent_exists = agentsManagerAgentExists(service->agents, mail);
	bool client_exists = clientsManagerClientExists(service->clients, mail);
	if ((!agent_exists) && (!client_exists)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_DOES_NOT_EXIST;
	} else if ((search_for_client && agent_exists) ||
			   ((!search_for_client) && client_exists)) {
		emailDestroy(mail);
		return MTM_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE;
	}
	return MTM_SERVICE_SUCCESS;
}

/**
* convertClientManagerResult: Converts a ClientsManagerResult to
* MTMServiceResult.
*
* @param value the ClientsManagerResult.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult convertClientManagerResult(ClientsManagerResult value){
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
				result = MTM_SERVICE_APARTMENT_SERVICE_ALREADY_EXISTS;
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
* convertEmailResult: Converts an EmailResult to
* MTMServiceResult.
*
* @param value the EmailResult.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult convertEmailResult(EmailResult value) {
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
* convertEmailResult: Converts an OfferManagerResult to
* MTMServiceResult.
*
* @param value the OfferManagerResult.
*
* @return the matching MTMServiceResult
*/
static MTMServiceResult convertOffersManagerResult(OfferManagerResult value) {
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
