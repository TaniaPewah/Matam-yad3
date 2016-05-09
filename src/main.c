#include <stdlib.h>
#include <stdio.h>
#include "apartment_service.h"
#include "aux_macros.h"
#include "yad3Program.h"

/*int RunEmailTest();
int RunAgentTest();
int	RunAgentManagerTest();
int RunClientTest();
int RunClientManagerTest();
int RunClientPurchaseBillTest();
int RunOfferTest();
int RunAgentDetailsTest();
int RunYad3ServiceTest();
int RunOffersManagerTest();*/

int main(int argc, char *argv[]) {
	char a0[] = "stam.exe";
	char a1[] = "-i";
	char a2[] = "C:\\Users\\binder\\Downloads\\Technion\\test\\in.in";
	char a3[] = "-o";
	char a4[] = "C:\\Users\\binder\\Downloads\\Technion\\test\\out.out";
	char* values[] = { a0, a1, a2, a3, a4};

	//Yad3Program prog = yad3ProgramCreate(argv, argc);
	Yad3Program prog = yad3ProgramCreate(values, 5);
	if (prog != NULL) {
		yad3ProgramRun(prog);
		yad3ProgramDestroy(prog);
	}

	/*for (int i =0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}*/

	/*RunEmailTest();
	RunAgentTest();
	RunAgentDetailsTest();
	RunAgentManagerTest();
	RunClientPurchaseBillTest();
	RunClientTest();
	RunClientManagerTest();
	RunOfferTest();
	RunYad3ServiceTest();
	RunOffersManagerTest();*/
	return 0;
}
