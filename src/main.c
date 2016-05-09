#include <stdlib.h>
#include <stdio.h>
#include "apartment_service.h"
#include "aux_macros.h"

int RunEmailTest();
int RunAgentTest();
int	RunAgentManagerTest();
int RunClientTest();
int RunClientManagerTest();
int RunClientPurchaseBillTest();
int RunOfferTest();
int RunAgentDetailsTest();
int RunMtmServiceTest();
int RunOffersManagerTest();

int main(int argc, char *argv[]) {

	/*for (int i =0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}*/

	RunEmailTest();
	RunAgentTest();
	RunAgentDetailsTest();
	RunAgentManagerTest();
	RunClientPurchaseBillTest();
	RunClientTest();
	RunClientManagerTest();
	RunOfferTest();
	RunMtmServiceTest();
	RunOffersManagerTest();
	return 0;
}
