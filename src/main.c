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

typedef void* Element;

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
}

void freeElement( Element element ){

	 free( element );
	 element = NULL;
}

Element copyElement( Element element ){

	void *temp = malloc( sizeof( *element ) );
	memcpy( temp, element, sizeof( *element ) );

	return temp;
}

bool list_mtm_TEST() {

	bool final = true;

	Element el1 = malloc( sizeof( element ));
	Element el2 = malloc( sizeof( element ));
	Element el3 = malloc( sizeof( element ));

	if( el1 && el2 && el3  ) {

		( (element*) el1 )->id = 1;
		( (element*) el2 )->id = 2;
		( (element*) el3 )->id = 3;

		List list = NULL;

		TEST_EQUALS(final, -1, listGetSize( list ));

		list = listCreate( copyElement, freeElement );

		TEST_EQUALS(final, 0, listGetSize( list ));
		TEST_EQUALS(final, NULL, listGetCurrent( list ));
		TEST_EQUALS(final, NULL, listGetFirst( list ));
		TEST_EQUALS(final, NULL, listGetNext( list ));

		ListResult r = listInsertFirst( list, el1 );

		TEST_EQUALS(final, LIST_SUCCESS, r );

		TEST_EQUALS(final, 0 ,compareElementId( listGetFirst( list ), el1 ) );
	}

	freeElement( el1 );
	freeElement( el2 );
	freeElement( el3 );

	return final;
}

int main(){

	return 0;
}

