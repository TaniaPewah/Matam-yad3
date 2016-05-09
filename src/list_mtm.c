#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list_mtm.h"

#define NULL_LIST_SIZE -1

typedef struct ListItem_t *ListItem;

struct ListItem_t {
	ListElement Element;
	ListItem Next;
};

struct List_t {
	CopyListElement copyElementFunc;
	FreeListElement freeElementFunc;
	ListItem First;
	ListItem Current;
};

static ListItem CreateListItem(CopyListElement copy, ListElement element);
static void DestroyListItem(FreeListElement freeElement, ListItem item);
static int GetListLength(List list);

/**
* Sets the internal iterator to the first element and retrieves it.
*
* The list has an internal iterator (also called current element) to allow
* iteration over the list's elements. This function sets the iterator to point
* to the first element in the list and return it.
* Use this to start iteraing over the list, searching from the beginning of
* the list and/or get the first element in the list.
* (To continue iteration use listGetNext)
* @code
* void f(List list) {
*   ListElement first = listGetFirst(list);
*   printf("The first element is at address %x\n", first);
* }
* @endcode
*
* @param list The list for which to set the iterator and return the first
* element.
* @return
* NULL is a NULL pointer was sent or the list is empty.
* The first element of the list otherwise
*/
ListElement listGetFirst(List list) {
	ListElement first = NULL;
	if ((list != NULL) && (list->First != NULL)) {
		first = list->First->Element;
		list->Current = list->First;
	}
	return first;
}

/**
* Returns the number of elements in a list
*
* @param list The target list which size is requested.
* @return
* -1 if a NULL pointer was sent.
* Otherwise the number of elements in the list.
*/
int listGetSize(List list) {
	int size = NULL_LIST_SIZE;
	if (list != NULL) size = GetListLength(list);
	return size;
}

/**
 * Returns the number of elements in the target list.
 *
 * @param list Target list to count its elements
 * @return
 * The list elements count.
 */
static int GetListLength(List list) {
	int size = 0;
	ListItem curItem = list->First;
	while (curItem != NULL) {
		size++;
		curItem = curItem->Next;
	}
	return size;
}

/**
* Advances the list's iterator to the next element and return it
*
* @param list The list for which to advance the iterator
* @return
* NULL if reached the end of the list, the iterator is at an invalid state or
* a NULL sent as argument
* The next element on the list in case of success
*/
ListElement listGetNext(List list) {
	ListElement next = NULL;
	if ((list != NULL) && (list->Current != NULL))
	{
		if (list->Current->Next != NULL) next = list->Current->Next->Element;
		list->Current = list->Current->Next;
	}
	return next;
}

/**
* Returns the current element (pointed by the iterator)
*
* @param list The list for which to get the iterator
* @return
* NULL if the iterator is at an invalid state or a NULL sent as argument
* The current element on the list in case of success
*/
ListElement listGetCurrent(List list) {
	return (((list != NULL) && (list->Current != NULL)) ?
			list->Current->Element : NULL);
}

/**
* Allocates a new List.
*
* Creates a new empty list. This function receives the functions which will be
* used for copying elements into the list and freeing them when needed.
* For example, in order to create a list of strings one need to define these
* two functions:
* @code
* ListElement copyString(ListElement str);
* void freeString(ListElement str);
* @endcode
* And then create a list as follows:
* @code
* List listOfStrings = listCreate(copyString, freeString);
* @endcode
*
* @param copyElement Function pointer to be used for copying elements into
* the list or when copying the list.
* @param freeElement Function pointer to be used for removing elements from
* the list.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new List in case of success.
*/
List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
	if ((copyElement == NULL) || (freeElement == NULL)) return NULL;
	List list = malloc(sizeof(*list));
	if (list == NULL) return NULL;
	list->copyElementFunc = copyElement;
	list->freeElementFunc = freeElement;
	list->First = NULL;
	list->Current = NULL;
	return list;
}

/**
* Creates a copy of target list.
*
* The new copy will contain all the elements from the source list in the same
* order and will use the same functions as the original list for copying and
* freeing elements.
*
* @param list The target list to copy
* @return
* NULL if a NULL was sent or a memory allocation failed.
* A List containing the same elements with same order as list otherwise.
*/
List listCopy(List list) {
	List copy = NULL;
	if (list != NULL) {
		copy = listCreate(list->copyElementFunc, list->freeElementFunc);
	}
	if ((copy != NULL) && (list->First != NULL)) {
		ListItem item_to_copy = list->First;
		while ((item_to_copy != NULL) &&  (copy != NULL)) {
			if (listInsertLast(copy, item_to_copy->Element) == LIST_SUCCESS) {
				if (list->Current == item_to_copy){
					int size = GetListLength(copy);
					listGetFirst(copy);
					for (int i = 1; i < size; i++) {
						listGetNext(copy);
					}
				}
				item_to_copy = item_to_copy->Next;
			} else {
				listDestroy(copy);
				copy = NULL;
			}
		}
	}
	return copy;
}

/**
 * Adds a new element to the list, the new element will be the first element.
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertFirst( List list, ListElement element ) {
	if ((list == NULL) || (element == NULL)) return LIST_NULL_ARGUMENT;
	ListItem new_list_item = CreateListItem(list->copyElementFunc, element);
	if (new_list_item == NULL) return LIST_OUT_OF_MEMORY;
	new_list_item->Next = list->First;
	list->First = new_list_item;
	return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be the last element
 *
 * @param list The list for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertLast( List list, ListElement element ) {
	if ((list == NULL) || (element == NULL)) return LIST_NULL_ARGUMENT;
	ListItem new_list_item = CreateListItem(list->copyElementFunc, element);
	if (new_list_item == NULL) return LIST_OUT_OF_MEMORY;
	if (list->First == NULL) {
		list->First = new_list_item;
	} else {
		ListItem last_item = list->First;
		while ( last_item->Next != NULL ) {

			last_item = last_item->Next;
		}
		last_item->Next = new_list_item;
	}
	return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be place right before
 * the current element (As pointed by the inner iterator of the list)
 *
 * @param list The list for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertBeforeCurrent( List list, ListElement element ) {
	if ((list == NULL) || (element == NULL)) return LIST_NULL_ARGUMENT;
	if (list->Current == NULL) return LIST_INVALID_CURRENT;
	ListItem new_list_item = CreateListItem(list->copyElementFunc, element);
	if (new_list_item == NULL) return LIST_OUT_OF_MEMORY;
	if (list->First == list->Current) {
		new_list_item->Next = list->First;
		list->First = new_list_item;
	} else {
		ListItem item_before_current = list->First;
		while (item_before_current->Next != list->Current) {
			item_before_current = item_before_current->Next;
		}
		new_list_item->Next = item_before_current->Next;
		item_before_current->Next = new_list_item;
	}
	return LIST_SUCCESS;
}

/**
 * Adds a new element to the list, the new element will be place right after
 * the current element (As pointed by the inner iterator be of the list)
 *
 * @param list The list for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function which is stored in the list
 * @return
 * LIST_NULL_ARGUMENT if a NULL was sent as list
 * LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
 * an element failed)
 * LIST_SUCCESS the element has been inserted successfully
 */
ListResult listInsertAfterCurrent(List list, ListElement element) {
	if ((list == NULL) || (element == NULL)) return LIST_NULL_ARGUMENT;
	if (list->Current == NULL) return LIST_INVALID_CURRENT;
	ListItem new_list_item = CreateListItem(list->copyElementFunc, element);
	if (new_list_item == NULL) return LIST_OUT_OF_MEMORY;
	new_list_item->Next = list->Current->Next;
	list->Current->Next = new_list_item;
	return LIST_SUCCESS;
}

/**
 * Creates a new ListItem that contains a copy of the given ListElement.
 *
 * @param copy Function pointer to be used for copying the element.
 * @param element The element to insert. A copy of the element will be
 * inserted as supplied by the copying function
 * @return
 * The newly created ListItem, or NULL in case of allocation failure (Meaning
 * the function for copying an element or allocating a ListItem struct failed)
 */
static ListItem CreateListItem(CopyListElement copy, ListElement element) {
	ListItem item = malloc(sizeof(*item ));
	if (item == NULL) return NULL;
	item->Next = NULL;
	item->Element = copy(element);
	if (item->Element == NULL) {
		free(item);
		item = NULL;
	}
	return item;
}

/**
* Creates a new filtered copy of a list.
*
* This creates a new list with only the elements for which the filtering
* function returned true.
*
* For example, the following code creates a new list, given a list of strings
* containing only the strings which are longer than 10 characters.
* @code
*
* bool isLongerThan(ListElement string, ListFilterKey key) {
*   return strlen(string) > *(int*)key;
* }
*
* List createFilteredList(List listOfStrings) {
*   int key = 10;
*   return listFilter(listOfStrings, isLongerThan, &key);
* }
* @endcode
*
* @param list The list for which a filtered copy will be made
* @param filterElement The function used for determining whether a given
* element should be in the resulting list or not.
* @param key Any extra values that need to be sent to the filtering function
* when called
* @return
* NULL if list or filterElement are NULL or a memory allocation failed.
* A List containing only elements from list which filterElement returned true
* for.
*/
List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
	if((list == NULL) && (filterElement == NULL)) return NULL;
	List filtered = listCopy(list);
	if (filtered == NULL) return NULL;
	if (filtered->First != NULL) {
		filtered->Current = filtered->First;
		while((filtered != NULL) && (filtered->Current != NULL)) {
			if (!filterElement(filtered->Current->Element, key)) {
				ListItem new_current = filtered->Current->Next;
				if (listRemoveCurrent(filtered) != LIST_SUCCESS) {
					listDestroy(filtered);
					filtered = NULL;
				} else {
					filtered->Current = new_current;
				}
			} else {
				filtered->Current = filtered->Current->Next;
			}
		}
	}
	if (filtered != NULL) filtered->Current = filtered->First;
	return filtered;
}

/**
 * Removes the currently pointed element of the list using the stored freeing
 * function
 *
 * @param list The list for which the current element will be removed
 * @return
 * LIST_NULL_ARGUMENT if list is NULL
 * LIST_INVALID_CURRENT if the current pointer of the list is in invalid state
 * LIST_SUCCESS the current element was removed successfully
 */
ListResult listRemoveCurrent(List list) {
	if (list == NULL) return LIST_NULL_ARGUMENT;
	if (list->Current == NULL) return LIST_INVALID_CURRENT;
	if (list->First == list->Current) {
		list->First = list->Current->Next;
	} else {
		ListItem toRemoveNext = list->First;
		while (toRemoveNext->Next != list->Current) {
			toRemoveNext = toRemoveNext->Next;
		}
		toRemoveNext->Next = toRemoveNext->Next->Next;
	}
	DestroyListItem(list->freeElementFunc, list->Current);
	list->Current = NULL;
	return LIST_SUCCESS;
}

/**
 * Sorts the list according to the given function, using a max sort sorting
 * method.
 *
 * For example, the following code will sort a list of integers according to
 * their distance from 0.
 * @code
 * int closerTo(ListElement num1, ListElement num2, ListSortKey value) {
 *   int distance1 = abs(*(int*)num1 - *(int*)value);
 *   int distance2 = abs(*(int*)num2 - *(int*)value);
 *   return distance1 - distance2;
 * }
 *
 * void sortInts(List listOfInts) {
 *   listSort(listOfInts, closerTo);
 * }
 * @endcode
 *
 * @param list the target list to sort
 * @param compareElement A comparison function as defined in the type
 * CompareListElements. This function should return an integer indicating the
 * relation between two elements in the list
 *
 * @return
 * LIST_NULL_ARGUMENT if list or compareElement are NULL
 * LIST_OUT_OF_MEMORY if a memory allocation failed, the list will be intact
 * in this case.
 * LIST_SUCCESS if sorting completed successfully.
 */
ListResult listSort( List list, CompareListElements compareElement ) {
	if ((list == NULL) || (compareElement == NULL)) return LIST_NULL_ARGUMENT;
	int list_size = listGetSize(list);
	if (list_size > 1) {
		for (int i = list_size - 1; list_size > 1; list_size--) {
			ListItem current = list->First;
			for (int j = 0; j < i; j++) {
				if (compareElement(current->Element, current->Next->Element)
						< 0) {
					ListElement element = current->Element;
					current->Element = current->Next->Element;
					current->Next->Element = element;
				}
				current = current->Next;
			}
		}
	}
	return LIST_SUCCESS;
}

/**
 * Removes all elements from target list.
 *
 * The elements are deallocated using the stored freeing function
 * @param list Target list to remove all element from
 * @return
 * LIST_NULL_ARGUMENT - if a NULL pointer was sent.
 * LIST_SUCCESS - Otherwise.
 */
ListResult listClear(List list) {
	if (list == NULL) return LIST_NULL_ARGUMENT;
	ListItem current = list->First;
	list->First = NULL;
	while (current != NULL) {
		ListItem temp = current;
		current = current->Next;
		DestroyListItem(list->freeElementFunc, temp);
		temp = NULL;
	}
	list->Current = NULL;
	return LIST_SUCCESS;
}

/**
 * DestroyListItem: Deallocates an existing ListItem. Clears the element by
 * using the given free function.
 *
 * @param freeElement Function pointer to be used for removing the element of
 * the ListItem.
 * @param item The ListItem to Deallocate.
 */
static void DestroyListItem(FreeListElement freeElement, ListItem item) {
	if (item != NULL) {
		if (item->Element != NULL) {
			freeElement(item->Element);
		}
		free(item);
		item = NULL;
	}
}

/**
* listDestroy: Deallocates an existing list. Clears all elements by using the
* stored free function.
*
* @param list Target list to be deallocated. If list is NULL nothing will be
* done
*/
void listDestroy( List list ) {
	if ( listClear(list) == LIST_SUCCESS) {
		free( list );
		list = NULL;
	}
}
