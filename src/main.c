#include <stdlib.h>
#include <stdio.h>
#include "apartment_service.h"
#include "aux_macros.h"

int RunEmailTest();
//int RunAgentTest();
int	RunAgentManagerTest();
int RunClientTest();
int RunClientManagerTest();
int RunClientPurchaseBillTest();
int RunOfferTest();
int RunAgentDetailsTest();
int RunMtmServiceTest();

int main(){
	RunEmailTest();
	//RunAgentTest();
	RunAgentDetailsTest();
	RunAgentManagerTest();
	RunClientTest();
	RunClientManagerTest();
	RunClientPurchaseBillTest();
	RunOfferTest();
	RunMtmServiceTest();
	return 0;
}
