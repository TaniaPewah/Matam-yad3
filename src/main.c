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

int main(){
	RunEmailTest();
	RunAgentTest();
	RunAgentManagerTest();
	RunClientTest();
	RunClientManagerTest();
	RunClientPurchaseBillTest();
	RunOfferTest();
	return 0;
}
