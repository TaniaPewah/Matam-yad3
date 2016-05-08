#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "yad3Program.h"
#include "mtmService.h"
#include "mtm_ex2.h"

#define COMMENT_SIGN "#"
#define COMMAND_SAPARATOR "\t"
#define USER_REALTOR "realtor"
#define USER_CUSTOMER "customer"
#define USER_REPORTER "report"
#define ACTION_ADD_USER "add"
#define ACTION_ADD_SERVICE "add_apartment_service"
#define ACTION_ADD_APARTMENT "add_apartment"
#define ACTION_REMOVE_USER "remove"
#define ACTION_REMOVE_SERVICE "remove_apartment_service"
#define ACTION_REMOVE_APARTMENT "remove_apartment"
#define ACTION_PURCHASE "purchase"
#define ACTION_MAKE_OFFER "make_offer"
#define ACTION_RESPOND_OFFER "respond_to_offer"
#define REPORT_RELEVENT_REALTORS "relevant_realtors"
#define REPORT_PAYING_CUSTOMERS "most_paying_customers"
#define REPORT_SIGNIFICANT_REALTORS "significant_realtors"

typedef void(*RunCommandFunction)(MTMService service, void** params);
typedef void(*DistroyCommandFunction)(void** params);

struct yad3Program_t {
	MTMService service;
};

struct yad3Command_t {
	RunCommandFunction commad;
	DistroyCommandFunction distroy;
	void** parameters;
};

Yad3Program yad3ProgramCreate() {
	MTMService new_service = mtmServiceCreate();
	if (new_service == NULL) return NULL;
	Yad3Program program = malloc(sizeof(*program));
	if (program == NULL) {
		mtmServiceDestroy(new_service);
		return NULL;
	}
	program->service = new_service;
	return program;
}

void yad3ProgramDestroy(Yad3Program program) {
	if (program) {
		mtmServiceDestroy(program->service);
	}
}






