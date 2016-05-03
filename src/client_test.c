//#include <stdlib.h>
//#include <stdio.h>
//#include <assert.h>
//#include <string.h>
//#include <stdbool.h>
//#include "test_utilities.h"
//#include "client.h"
//
//int RunClientTest();
//static bool testClientCreate();
//static bool testClientGetersAndSetters();
//
////int main() {
//int RunClientTest() {
//	RUN_TEST(testClientCreate);
//	RUN_TEST(testClientGetersAndSetters);
//	return 0;
//}
//
///**
// * Test create method
// */
//static bool testClientCreate() {
//	char* bad_mail = "babaganosh";
//	char* good_mail = "baba@ganosh";
//	Client client;
//	ASSERT_TEST(clientCreate(NULL,5,5,5, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(bad_mail,5,5,5, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(good_mail,-5, 5, 5, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(good_mail,5, -5, 5, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(good_mail,5, 5, -5, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(good_mail,0, 5, 5, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(good_mail,5, 0, 5, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(good_mail,5, 5, 0, &client)
//			== CLIENT_INVALID_PARAMETERS);
//	ASSERT_TEST(clientCreate(good_mail,5, 5, 5, &client)
//			== CLIENT_SUCCESS);
//	ASSERT_TEST(client != NULL);
//	clientDestroy(client);
//	return true;
//}
//
//static bool testClientGetersAndSetters() {
//	char* good_mail = "baba@ganosh";
//	Client client;
//	clientCreate(good_mail, 5, 6, 7, &client);
//	ASSERT_TEST(clientGetMail(NULL) == NULL);
//	ASSERT_TEST(clientGetMinArea(NULL) == NO_CLIENT_VAL);
//	ASSERT_TEST(clientGetMinRooms(NULL) == NO_CLIENT_VAL);
//	ASSERT_TEST(clientGetMaxPrice(NULL) == NO_CLIENT_VAL);
//	ASSERT_TEST(clientGetTotalPayments(NULL) == NO_CLIENT_VAL);
//	ASSERT_TEST(strcmp(clientGetMail(client), good_mail) == 0);
//	ASSERT_TEST(clientGetMinArea(client) == 5);
//	ASSERT_TEST(clientGetMinRooms(client) == 6);
//	ASSERT_TEST(clientGetMaxPrice(client) == 7);
//	ASSERT_TEST(clientGetTotalPayments(client) == 0);
//	clientAddPayment(client, 50);
//	ASSERT_TEST(clientGetTotalPayments(client) == 50);
//	clientDestroy(client);
//	return true;
//}
//
