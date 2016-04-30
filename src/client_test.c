#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "client.h"

int RunClientTest();
static bool testClientCreate();

//int main() {
int RunClientTest() {
	RUN_TEST(testClientCreate);
	return 0;
}

/**
 * Test create method
 */
static bool testClientCreate() {
	char* bad_mail = "babaganosh";
	char* good_mail = "baba@ganosh";
	Client client;

	// ClientResult clientCreate(const char* email, int apartment_min_area,
	//		int apartment_min_rooms, int apartment_max_price, Client* result)
	ASSERT_TEST(clientCreate(NULL,5,5,5, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(bad_mail,5,5,5, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(good_mail,-5, 5, 5, &client)
			== CLIENT_INVALID_PARAMETERS);
	return true;
}
