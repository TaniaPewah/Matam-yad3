#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "yad3Program.h"
#include "utilities.h"
#include "yad3Service.h"
#include "mtm_ex2.h"

#define COMMENT_SIGN '#'
#define COMMAND_SAPARATOR_1 '\t'
#define COMMAND_SAPARATOR_2 ' '
#define COMMAND_SAPARATORS " \t"
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
#define END_OF_STRING '\0'

typedef enum  {
	READ = 1,
	WRITE = 2,
	APPEND = 3
} MTMFileMode;

struct yad3Program_t {
	Yad3Service service;
	FILE* input;
	FILE* output;
};

static bool checkProgramParameters(char *input[], int count);
static char* GetInputsPaths(char *input[], int count, bool find_input);
static Yad3Program allocateYad3Program(FILE *input, FILE *output);
static bool openFile(char* path, MTMFileMode mode, FILE** output);
static void closeFile(FILE* output);
static void writeToErrorOutStream(MtmErrorCode code);

static bool RunCommand(char* command, Yad3Program program);
static bool RunReporterCommand(char** params, Yad3Program program);
static bool RunPayingCustumersReport(char** params, Yad3Program program);

static bool RunRealtorCommand(char** params, Yad3Program program);
static bool RunAddRealtor(char** params, Yad3Program program);
static bool RunRemoveRealtor(char** params, Yad3Program program);
static bool RunRealtorAddApartmentService(char** params, Yad3Program program);
static bool RunRealtorRemoveApartmentService(char** params,
	Yad3Program program);
static bool RunRealtorAddApartmentToRealtor(char** params,
	Yad3Program program);
static bool RunRemoveApartmentFromRealtor(char** params, Yad3Program program);

static bool RunCustomerCommand(char** params, Yad3Program program);
static bool RunAddCustumer(char** params, Yad3Program program);
static bool RunRemoveCustumer(char** params, Yad3Program program);
static bool RunCustumerPurchase(char** params, Yad3Program program);
static bool RunMakeOffer(char** params, Yad3Program program);

static bool HandleResult(Yad3ServiceResult result);
MtmErrorCode ConvertYad3ServiceResult(Yad3ServiceResult value);
static bool splitString(char* string, int *size, char*** out_matrix);
//static char** splitString(char* string, int *size);
//static char* getSubString(char* str, int start_index, int end_index);
//static void matrixDestroy(char** matrix, int size);

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
	Yad3Service new_service = yad3ServiceCreate();
	if (new_service == NULL) return NULL;
	Yad3Program program = malloc(sizeof(*program));
	if (program == NULL) {
		yad3ServiceDestroy(new_service);
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
		yad3ServiceDestroy(program->service);
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

/*
* yad3ProgramRun: runs the Yad3Program.
*
* @param program program to destroy
*/
void yad3ProgramRun(Yad3Program program) {
	if (program == NULL) return;
	char buffer[MAX_LEN] = "";
	bool should_continue = true;

	while (should_continue && fgets(buffer, MAX_LEN,
			(program->input != NULL ? program->input : stdin)) != NULL) {
		should_continue = RunCommand(buffer, program);
	}
}

/*
 * Runs a command recived from the defined input stream
 */
static bool RunCommand(char* command, Yad3Program program) {
	if (command[0] == COMMENT_SIGN) return true;
	int size;
	char** params;
	bool should_continue = true;
	if (!splitString(command, &size, &params)) {
		writeToErrorOutStream(MTM_OUT_OF_MEMORY);
		should_continue = false;
	}else if (params == NULL) {
		should_continue = true;
	} else if (areStringsEqual(params[0], USER_CUSTOMER)) {
		should_continue = RunCustomerCommand(params, program);
	} else if (areStringsEqual(params[0], USER_REALTOR)) {
		should_continue = RunRealtorCommand(params, program);
	} else if (areStringsEqual(params[0], USER_REPORTER)) {
		should_continue = RunReporterCommand(params, program);
	} else {
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		should_continue = false;
	}
	free(params);
	return should_continue;
}

static bool RunRealtorCommand(char** params, Yad3Program program) {
	if (areStringsEqual(params[1], ACTION_ADD_USER)) {
		return RunAddRealtor(params, program);
	} else if (areStringsEqual(params[1], ACTION_REMOVE_USER)) {
		return RunRemoveRealtor(params, program);
	} else if (areStringsEqual(params[1], ACTION_ADD_SERVICE)) {
		return RunRealtorAddApartmentService(params, program);
	} else if (areStringsEqual(params[1], ACTION_REMOVE_SERVICE)) {
		return RunRealtorRemoveApartmentService(params, program);
	} else if (areStringsEqual(params[1], ACTION_ADD_APARTMENT)) {
		return RunRealtorAddApartmentToRealtor(params, program);
	} else if (areStringsEqual(params[1], ACTION_REMOVE_APARTMENT)) {
		return RunRemoveApartmentFromRealtor(params, program);
	} else if (areStringsEqual(params[1], ACTION_RESPOND_OFFER)) {
		// TODO : ADD to service!
		return true;
	} else {
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
	}
}

/*
 * Run AddRealtor command
*/
static bool RunAddRealtor(char** params, Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL) && (params[4] != NULL)) {
		Yad3ServiceResult result = yad3ServiceAddAgent(program->service,
			params[2], params[3], stringToInt(params[4]));
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Run RunRemoveRealtor command
*/
static bool RunRemoveRealtor(char** params, Yad3Program program) {
	if (params[2] != NULL) {
		Yad3ServiceResult result = yad3ServiceRemoveAgent(program->service,
			params[2]);
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Run RealtorAddApartmentService command
 */
static bool RunRealtorAddApartmentService(char** params, Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL) && (params[4] != NULL)) {
			Yad3ServiceResult result = yad3ServiceAddServiceToAgent(program->
				service, params[2], params[3], stringToInt(params[4]));
			return HandleResult(result);
		}
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
}

/*
 * Run RealtorAddApartmentService command
 */
static bool RunRealtorRemoveApartmentService(char** params,
	Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL)) {
			Yad3ServiceResult result = yad3ServiceRemoveServiceFromAgent(
				program->service, params[2], params[3]);
			return HandleResult(result);
		}
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
}

/*
 * Run AddRealtor command
*/
static bool RunRealtorAddApartmentToRealtor(char** params,
		Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL) && (params[4] != NULL)) {
		Yad3ServiceResult result = yad3ServiceAddApartmentToAgent(
			program->service, params[2], params[3], stringToInt(params[4]),
			stringToInt(params[5]), stringToInt(params[6]),
			stringToInt(params[7]), params[8]);
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Run RemoveApartmentFromRealtor command
 */
static bool RunRemoveApartmentFromRealtor(char** params, Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL) && (params[4] != NULL)) {
			Yad3ServiceResult result = yad3ServiceRemoveApartmentFromAgent(
				program->service, params[2], params[3],
				stringToInt(params[4]));
			return HandleResult(result);
		}
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
}

/*
 * Runs one of the possible Customer command
*/
static bool RunCustomerCommand(char** params, Yad3Program program) {
	if (areStringsEqual(params[1], ACTION_ADD_USER)) {
		return RunAddCustumer(params, program);
	} else if (areStringsEqual(params[1], ACTION_REMOVE_USER)) {
		return RunRemoveCustumer(params, program);
	} else if (areStringsEqual(params[1], ACTION_PURCHASE)) {
		return RunCustumerPurchase(params, program);
	} else if (areStringsEqual(params[1], ACTION_MAKE_OFFER)) {
		return RunMakeOffer(params, program);
	} else {
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
	}
}

/*
 * Run AddCustumer command
*/
static bool RunAddCustumer(char** params, Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL) && (params[4] != NULL)
			&& (params[5] != NULL)) {
		Yad3ServiceResult result = yad3ServiceAddClient(program->service,
			params[2], stringToInt(params[3]), stringToInt(params[4]),
			stringToInt(params[5]));
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Run RemoveCustumer command
*/
static bool RunRemoveCustumer(char** params, Yad3Program program) {
	if (params[2] != NULL) {
		Yad3ServiceResult result = yad3ServiceRemoveClient(program->service,
			params[2]);
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Run RunCustumerPurchase command
*/
static bool RunCustumerPurchase(char** params, Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL) && (params[4] != NULL)
			&& (params[5] != NULL)) {
		Yad3ServiceResult result = yad3ServiceClientPurchaseApartment
			(program->service,  params[2], params[3], params[4],
			stringToInt(params[5]));
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Run RunMakeOffer command
*/
static bool RunMakeOffer(char** params, Yad3Program program) {
	if ((params[2] != NULL) && (params[3] != NULL) && (params[4] != NULL)
			&& (params[5] != NULL) && (params[6] != NULL)) {
		Yad3ServiceResult result = yad3ServiceMakeClientOffer(program->service,
			params[2], params[3], params[4], stringToInt(params[5]),
			stringToInt(params[6]));
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Runs all the possible Reporter commands
*/
static bool RunReporterCommand(char** params, Yad3Program program) {
	if (areStringsEqual(params[1], REPORT_RELEVENT_REALTORS)) {
		// TODO : ADD to service!
		return true;
	} else if (areStringsEqual(params[1], REPORT_SIGNIFICANT_REALTORS)) {
		// TODO : ADD to service!
		return true;
	}else if (areStringsEqual(params[1], REPORT_PAYING_CUSTOMERS)) {
		RunPayingCustumersReport(params, program);
		return true;
	} else {
		writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return false;
	}
}

/*
 * Run print most paying costumers report command
*/
static bool RunPayingCustumersReport(char** params, Yad3Program program) {
	if ((params[2] != NULL)) {
		Yad3ServiceResult result = yad3ServiceMostPayingCustomers(
			program->service, stringToInt(params[2]), program->output);
		return HandleResult(result);
	}
	writeToErrorOutStream(MTM_INVALID_COMMAND_LINE_PARAMETERS);
	return false;
}

/*
 * Handles the code from service, returns if should continue or not
*/
static bool HandleResult(Yad3ServiceResult result) {
	if (result == YAD3_SERVICE_SUCCESS) return true;
	MtmErrorCode code = ConvertYad3ServiceResult(result);
	writeToErrorOutStream(code);
	return (code == MTM_OUT_OF_MEMORY || code == MTM_CANNOT_OPEN_FILE ||
		code == MTM_INVALID_COMMAND_LINE_PARAMETERS);
}

/*
 * Convert Yad3ServiceResult to MtmErrorCode
*/
MtmErrorCode ConvertYad3ServiceResult(Yad3ServiceResult value) {
	MtmErrorCode result;
	switch (value) {
		case YAD3_SERVICE_OUT_OF_MEMORY: {
			result = MTM_OUT_OF_MEMORY;
			break; }
		case YAD3_SERVICE_INVALID_PARAMETERS: {
			result = MTM_INVALID_PARAMETERS;
			break; }
		case YAD3_SERVICE_EMAIL_ALREADY_EXISTS: {
			result = MTM_EMAIL_ALREADY_EXISTS;
			break; }
		case YAD3_SERVICE_EMAIL_DOES_NOT_EXIST: {
			result = MTM_EMAIL_DOES_NOT_EXIST;
			break; }
		case YAD3_SERVICE_EMAIL_WRONG_ACCOUNT_TYPE: {
			result = MTM_EMAIL_WRONG_ACCOUNT_TYPE;
			break; }
		case YAD3_SERVICE_ALREADY_REQUESTED: {
			result = MTM_ALREADY_REQUESTED;
			break; }
		case YAD3_SERVICE_NOT_REQUESTED: {
			result = MTM_NOT_REQUESTED;
			break; }
		case YAD3_SERVICE_APARTMENT_SERVICE_ALREADY_EXISTS: {
			result = MTM_APARTMENT_SERVICE_ALREADY_EXISTS;
			break; }
		case YAD3_SERVICE_APARTMENT_SERVICE_DOES_NOT_EXIST: {
			result = MTM_APARTMENT_SERVICE_DOES_NOT_EXIST;
			break; }
		case YAD3_SERVICE_APARTMENT_SERVICE_FULL: {
			result = MTM_APARTMENT_SERVICE_FULL;
			break; }
		case YAD3_SERVICE_APARTMENT_ALREADY_EXISTS: {
			result = MTM_APARTMENT_ALREADY_EXISTS;
			break; }
		case YAD3_SERVICE_APARTMENT_DOES_NOT_EXIST: {
			result = MTM_APARTMENT_DOES_NOT_EXIST;
			break; }
		case YAD3_SERVICE_PURCHASE_WRONG_PROPERTIES: {
			result = MTM_PURCHASE_WRONG_PROPERTIES;
			break; }
		case YAD3_SERVICE_REQUEST_WRONG_PROPERTIES: {
			result = MTM_REQUEST_WRONG_PROPERTIES;
			break; }
		case YAD3_SERVICE_REQUEST_ILLOGICAL_PRICE: {
			result = MTM_REQUEST_ILLOGICAL_PRICE;
			break; }
		case YAD3_SERVICE_SUCCESS:
		default: {
			result = MTM_OUT_OF_MEMORY;
			break;
		}
	}
	return result;
}

/*
* commandSplit: splits the string to an array of sub string
* ignores eampty strings.
*
* @param string the string.
* @param size the new array size.
* @param value the split character.
*
* @return
* 	NULL if string is null or malloc failed; else returns the matrix
 */
static bool splitString(char* string, int *size, char*** out_matrix) {
	int actual_size = 8;
	int logical_size = 0;
	char** result = malloc(sizeof(char*) * actual_size);
	if (result == NULL) return NULL;
	char *token = strtok(string, COMMAND_SAPARATORS);
	while (token != NULL) {
		if (actual_size == logical_size) {
			actual_size += 6;
			char** new_array = realloc(result, actual_size);
			if (new_array == NULL) {
				free (result);
				return false;
			}
			result = new_array;
		}
		result[logical_size] = token;
		logical_size++;
	}
	char** new_array = NULL;
	if (logical_size > 0) {
		char** new_array = realloc(result, logical_size);
		if (new_array == NULL) {
			free (result);
			return false;
		}
	}
	free(result);
	*out_matrix = new_array;
	return true;
}



/*
* commandSplit: splits the string to an array of sub string
* ignores eampty strings.
*
* @param string the string.
* @param size the new array size.
* @param value the split character.
*
* @return
* 	NULL if string is null or malloc failed; else returns the matrix
 */
//static char** splitString(char* string, int *size) {
//    char** result = NULL;
//    int count = countChar(string, COMMAND_SAPARATOR_1) +
//    		countChar(string, COMMAND_SAPARATOR_2);
//    *size = count;
//    int start_index = 0, last_index = 0, current_index = 0, item_index = 0;
//    result = malloc(sizeof(char*) * count);
//    if (result == NULL) return NULL;
//    for (int i = 0; i < count; i++) result[i] = NULL;
//    while (string[current_index] != END_OF_STRING) {
//    	if ((string[current_index] != countChar(string, COMMAND_SAPARATOR_1))&&
//    		(string[current_index] != countChar(string, COMMAND_SAPARATOR_2))){
//    		last_index++;
//    	} else {
//    		if (start_index < last_index) {
//				result[item_index] = getSubString(string, start_index,
//						last_index);
//				item_index++;
//				if (result[item_index] == NULL) {
//					matrixDestroy(result, count);
//					return NULL;
//				}
//			}
//    		start_index = current_index + 1;
//    		last_index = current_index + 1;
//    	}
//    	current_index++;
//    }
//    if (start_index < last_index) {
//		result[item_index] = getSubString(string, start_index, last_index-1);
//		if (result[item_index] == NULL) {
//			matrixDestroy(result, count);
//			return NULL;
//		}
//	}
//    return result;
//}
//
///*
//* getSubString: returns a new sub string in the given range.
//*
//* @param string the string.
//* @param start_index the start_index.
//* @param end_index the end_index.
//*
//* @return
//* 	NULL if string is null or malloc failed or end_index - start_index <= 0;
//* 	else returns the string
// */
//static char* getSubString(char* str, int start_index, int end_index) {
//	if ((str == NULL) || (end_index - start_index <= 0)) return NULL;
//	char* new_string = malloc(sizeof(char) * ((end_index - start_index) + 2));
//	if (new_string == NULL) return NULL;
//	for (int i = 0; i < (end_index - start_index + 1); i++) {
//		new_string[i] = str[start_index + i];
//	}
//	new_string[end_index - start_index + 1] = END_OF_STRING;
//	return new_string;
//}
//
///*
//* matrixDestroy: Destroys the given matrixDestroy using free.
//*
//* @param matrix the matrix.
//* @param size the size.
// */
//static void matrixDestroy(char** matrix, int size) {
//	for (int i = 0; i < size; i++) {
//		free(matrix[i]);
//	}
//	free(matrix);
//}
