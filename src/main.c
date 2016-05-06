#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "apartment_service.h"
#include "aux_macros.h"

int RunEmailTest();
int	RunAgentManagerTest();
int RunClientTest();

int main(){
	RunEmailTest();
	RunClientTest();
	RunAgentManagerTest();
	return 0;
}
