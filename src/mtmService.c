#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "mtmService.h"
#include "email.h"
#include "clientsManager.h"
#include "agentsManager.h"

struct mtmService_t {
	ClientsManager clients;
	AgentsManager agents;
	//OffersManager offers
};

//ServiceResult mtmServiceAddClient(MTMService service, byte[] email_adress,
//		int minArea, int minRoom, int maxPrice) {

//}
