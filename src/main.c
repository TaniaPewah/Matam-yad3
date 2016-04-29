/*
 * main.c
 *
 *  Created on: Apr 28, 2016
 *      Author: Tania
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "aux_macros.h"
#include "list_mtm.h"

/*typedef void* Element;

typedef struct {

	int id;
	char* value;

} element;

int compareElementId(void *a, void *b) {

	int a_ = ( (element*) a )->id;
	int b_ = ( (element*) b )->id;

	if( a_ < b_ ) return 1;
	if( a_ == b_ ) return 0;

	else return -1;
}*/

void freeElement( void* element ){

	 //free( element );
	 element = NULL;
}

void* copyElement( void* element ){

	void *temp = malloc( sizeof( element ) );
	memcpy( temp, element, sizeof( element ) );
	return temp;
}

bool list_mtm_TEST() {

	bool final = true;

	int el1 = 1;
	int el2 = 2;
	int el3 = 3;

	List list = NULL;

	// List is null everything is null
	TEST_EQUALS(final, -1, listGetSize( list ));
	TEST_EQUALS(final, NULL, listGetCurrent( list ));
	TEST_EQUALS(final, NULL, listGetFirst( list ));
	TEST_EQUALS(final, NULL, listGetNext( list ));

	// list created, size zero everything else null
	list = listCreate( copyElement, freeElement );
	TEST_EQUALS(final, 0, listGetSize( list ));
	TEST_EQUALS(final, NULL, listGetCurrent( list ));
	TEST_EQUALS(final, NULL, listGetFirst( list ));
	TEST_EQUALS(final, NULL, listGetNext( list ));


	//inserting first element el1
	TEST_EQUALS(final, LIST_SUCCESS, listInsertFirst( list, &el1 ) );
	TEST_EQUALS(final, 1 , *((int*)listGetFirst( list )) );
	TEST_EQUALS(final, 1, listGetSize( list ));
	TEST_EQUALS(final, 1, *((int*)listGetCurrent( list )) );
	TEST_EQUALS(final, NULL, listGetCurrent( list ));
	TEST_EQUALS(final, NULL, listGetNext( list ));

	// inserting next element
	TEST_EQUALS(final, 1 , *((int*)listGetFirst( list )) );
	TEST_EQUALS(final, LIST_SUCCESS, listInsertAfterCurrent( list, &el2 ) );
	TEST_EQUALS(final, 1 ,  *((int*)listGetFirst( list )) );
	TEST_EQUALS(final, 2, listGetSize( list ));
	TEST_EQUALS(final, 2, *((int*)listGetNext( list )));


	/*
	listCreate
	listDestroy
	listCopy
	listFilter
	*/

	return final;
}

int main(){


	RUN_TEST( list_mtm_TEST );
	return 0;
}

