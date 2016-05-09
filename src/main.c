#include <stdlib.h>
#include <stdio.h>
#include "apartment_service.h"
#include "aux_macros.h"
#include "yad3Program.h"

int RunEmailTest();
int RunAgentTest();
int	RunAgentManagerTest();
int RunClientTest();
int RunClientManagerTest();
int RunClientPurchaseBillTest();
int RunOfferTest();
int RunAgentDetailsTest();
int RunMtmServiceTest();

int main(int argc, char *argv[]) {

	/* Yad3Program prog = yad3ProgramCreate(argv, argc);

	if (prog != NULL) {
		yad3ProgramRun(prog);
		yad3ProgramDestroy(prog);
	} */

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
	return 0;
}
