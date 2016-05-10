#include <stdlib.h>
#include <stdio.h>
#include "apartment_service.h"
#include "aux_macros.h"
#include "yad3Program.h"

int main(int argc, char *argv[]) {
	Yad3Program prog = yad3ProgramCreate(argv, argc);
	if (prog != NULL) {
		yad3ProgramRun(prog);
		yad3ProgramDestroy(prog);
	}
	return 0;
}
