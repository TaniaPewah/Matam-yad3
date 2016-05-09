#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "aux_macros.h"
#include "list_mtm.h"

int main() {
	RUN_TEST(test_listCreate);
	return 0;
}

ListElement copyIntager(ListElement element) {
	int* new_value = malloc(sizeof(*new_value));
	*new_value = *((int*)element);
	return new_value;
}

void freeIntager(ListElement element) {
	if (element != NULL) {
		free(element);
	}
}


bool test_listCreate() {
	bool final;
	List list = listCreate(NULL, NULL);
	TEST_EQUALS(final, NULL, list);
	List list = listCreate(copyIntager, NULL);
	TEST_EQUALS(final, NULL, list);
	List list = listCreate(NULL, freeIntager);
	TEST_EQUALS(final, NULL, list);
	List list = listCreate(copyIntager, freeIntager);
	TEST_DIFFERENT(final, NULL, list);
	listDestroy(list);
	return final;
}

