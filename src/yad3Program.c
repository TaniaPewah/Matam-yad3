#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include <stdbool.h>
#include "yad3Program.h"
#include "utilities.h"
#include "mtmService.h"
#include "mtm_ex2.h"

#define COMMENT_SIGN '#'
#define COMMAND_SAPARATOR '\t'
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

typedef enum  {
	READ = 1,
	WRITE = 2,
	APPEND = 3
} MTMFileMode;

struct yad3Program_t {
	MTMService service;
	FILE* input;
	FILE* output;
};

static bool checkProgramParameters(char *input[], int count);
static char* GetInputsPaths(char *input[], int count, bool find_input);
static Yad3Program allocateYad3Program(FILE *input, FILE *output);
static bool openFile(char* path, MTMFileMode mode, FILE** output);
static void closeFile(FILE* output);
static void writeToErrorOutStream(MtmErrorCode code);

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
Yad3Program yad3ProgramCreate(char *input_parameters[], int parameter_count) {
	if (!checkProgramParameters(input_parameters, parameter_count)) {
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return NULL;
	}
	char* input = GetInputsPaths(input_parameters, parameter_count, false);
	char* output = GetInputsPaths(input_parameters, parameter_count, true);
	FILE *out_file = NULL, *in_file = NULL;
	bool error = (!(output == NULL || openFile(output, WRITE, &out_file))) &&
				 (!(input == NULL || openFile(input, READ, &in_file)));
	if (error) {
		writeToErrorOutStream(MTM_CANNOT_OPEN_FILE);
		return NULL;
	}
	return (allocateYad3Program(in_file, out_file));
}

/*
* checkProgramParameters:
*
* 	method checks if the program input parameters are correct.
*
* 	correct parameters are:
* 	- only one parameter
* 	- three parameters, the third must be INPUT_SIGN or OUTPUT_SIGN
* 	- five parameters, the third and the fifth must be INPUT_SIGN and
* 		OUTPUT_SIGN (one must be OUTPUT_SIGN and the second must be INPUT_SIGN)
*
* @param input_parameters array of input parameters
* @param parameter_count size of parameters array
*
* @return
*	false if parameters are not good; else returns true.
*/
static bool checkProgramParameters(char *input[], int count) {
	if (count == 1) return true;
	if ((count == 3) &&
		((areStringsEqual(input[1], INPUT_SIGN)) ||
		 (areStringsEqual(input[1], OUTPUT_SIGN)))) return true;
	if ((count == 5) &&
		(((areStringsEqual(input[1], INPUT_SIGN)) &&
		  (areStringsEqual(input[3], OUTPUT_SIGN))) ||
		 ((areStringsEqual(input[1], OUTPUT_SIGN)) &&
		  (areStringsEqual(input[3], INPUT_SIGN))))) return true;
	return false;
}

/*
* GetInputFileAdress:
*
* 	method gets input/output address from program input parameters.
*
* @param input_parameters array of input parameters
* @param parameter_count size of parameters array
*
* @return
*	false if parameters are not good; else returns true.
*/
static char* GetInputsPaths(char *input[], int count, bool find_input) {
	if ((count == 3) &&
		(areStringsEqual(input[1], (find_input ? OUTPUT_SIGN : INPUT_SIGN)))) {
		return input[2];
	}
	if ((count == 5) &&
		(areStringsEqual(input[3], (find_input ? OUTPUT_SIGN : INPUT_SIGN)))) {
		return input[4];
	}
	return NULL;
}

/*
* allocateYad3Program:
*
* allocates a new yad3 programs parameters.
*
* @param input_parameters array of input parameters
* @param parameter_count size of parameters array
*
* @return
*	NULL in case of allocation error; else returns true.
*/
static Yad3Program allocateYad3Program(FILE *input, FILE *output) {
	MTMService new_service = mtmServiceCreate();
	if (new_service == NULL) return NULL;
	Yad3Program program = malloc(sizeof(*program));
	if (program == NULL) {
		mtmServiceDestroy(new_service);
		closeFile(input);
		closeFile(output);
		return NULL;
	}
	program->service = new_service;
	program->input = input;
	program->output = output;
	return program;
}

/*
* openFile:
*
* 	method opens a FILE* element to the given path.
*
* @param path the file path
* @param mode the file mode
* @param output pointer to save the FILE* element
*
* @return
*	false if parameters are not good or failed opening file; else returns true.
*/
static bool openFile(char* path, MTMFileMode mode, FILE** output) {
	if ((path != NULL) && (output != NULL)) {
		switch (mode){
			case READ : {
				*output = fopen(path, "r");
				break;
			}
			case WRITE : {
				*output = fopen(path, "w");
				break;
			}
			case APPEND : {
				*output = fopen(path, "a");
				break;
			}
			default : {
				*output = NULL;
				break;
			}
		}
		return (*output != NULL);
	}
	return false;
}

/*
* closeFile: method closes a FILE* element.
*
* @param output the file to close
*/
static void closeFile(FILE *output) {
	if (output != NULL) {
		fclose (output);
	}
}

/*
* yad3ProgramDestroy: destroys a Yad3Program.
*
* @param program program to destroy
*/
void yad3ProgramDestroy(Yad3Program program) {
	if (program) {
		closeFile(program->input);
		closeFile(program->output);
		mtmServiceDestroy(program->service);
	}
}

/*
* writeToErrorOutStream: writes a code to the error out stream.
*
* @param program program to destroy
*/
static void writeToErrorOutStream(MtmErrorCode code) {
	mtmPrintErrorMessage(stderr, code);
}

bool charArrayToComands( char* input_array, Yad3Program program ){

	if((char)input_array[0] == COMMENT_SIGN ) return false;

	//char** split_command = str_split( input_array, ' ');

	return true;
}


