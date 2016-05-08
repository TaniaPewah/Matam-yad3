#ifndef SRC_YAD3PROGRAM_H_
#define SRC_YAD3PROGRAM_H_

#include <stdbool.h>

typedef struct yad3Program_t *Yad3Program;
typedef struct yad3Command_t *Yad3Command;

#define OUTPUT_SIGN "-o"
#define INPUT_SIGN "-i"

/**
* Allocates Yad3Program.
*
* Creates a new Yad3Program. This function receives the input parameters of the
* process and retrieves the new program created.
*
* 	correct parameters are:
* 	- only one parameter
* 	- three parameters, the third must be INPUT_SIGN or OUTPUT_SIGN
* 	- five parameters, the third and the fifth must be INPUT_SIGN and
* 		OUTPUT_SIGN (one must be OUTPUT_SIGN and the second must be INPUT_SIGN)
*
* @param input_parameters input parameters.
* @param parameter_count input parameters count.
*
* @return
* 	NULL in case of wrong parameters or allocation error; else return true
*/
Yad3Program yad3ProgramCreate(char *input_parameters[], int parameter_count);

/*
* yad3ProgramDestroy: destroys a Yad3Program.
* if NULL is sent, nothing happens
*
* @param program program to destroy
*/
void yad3ProgramDestroy(Yad3Program program);

#endif /* SRC_YAD3PROGRAM_H_ */
