#ifndef SRC_YAD3PROGRAM_H_
#define SRC_YAD3PROGRAM_H_

#include <stdbool.h>

typedef struct yad3Program_t *Yad3Program;
typedef struct yad3Command_t *Yad3Command;

Yad3Program yad3ProgramCreate();

void yad3ProgramDestroy(Yad3Program program);

Yad3Command yad3CommandCreate(Yad3Program program, char* command);

void yad3CommandDestroy(Yad3Command command);

bool yad3CommandRun(Yad3Command command);

#endif /* SRC_YAD3PROGRAM_H_ */
