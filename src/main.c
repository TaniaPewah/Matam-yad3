#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "apartment_service.h"
#include "aux_macros.h"

int RunEmailTest();
int RunAgentTest();
int	RunAgentManagerTest();
int RunClientTest();
int RunClientPurchaseBillTest();

int main(){
	RunEmailTest();
	RunClientTest();
	RunClientPurchaseBillTest();
	RunAgentTest();
	RunAgentManagerTest();
	return 0;
}
