#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "client.h"
#include "email.h"

static bool testClientCreate();
static bool testClientGetersAndSetters();
static bool testClientCopy();

int RunClientTest() {
	RUN_TEST(testClientCreate);
	RUN_TEST(testClientGetersAndSetters);
	RUN_TEST(testClientCopy);
	return 0;
}

/**
 * Test create method
 */
static bool testClientCreate() {
	char* good_mail = "baba@ganosh";
	Email mail;
	emailCreate(good_mail, &mail);
	Client client;
	ASSERT_TEST(clientCreate(NULL,5,5,5, &client)
			== CLIENT_NULL_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,5, 5, 5, NULL)
			== CLIENT_NULL_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,-5, 5, 5, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,5, -5, 5, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,5, 5, -5, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,0, 5, 5, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,5, 0, 5, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,5, 5, 0, &client)
			== CLIENT_INVALID_PARAMETERS);
	ASSERT_TEST(clientCreate(mail,5, 5, 5, &client)
			== CLIENT_SUCCESS);
	ASSERT_TEST(client != NULL);
	clientDestroy(client);
	emailDestroy(mail);
	return true;
}

/**
 * Test geters and seters method
 */
static bool testClientGetersAndSetters() {
	char* mail_adress = "baba@ganosh";
	Email good_mail;
	emailCreate(mail_adress, &good_mail);
	Client client;
	clientCreate(good_mail, 5, 6, 7, &client);
	ASSERT_TEST(clientGetMail(NULL) == NULL);
	ASSERT_TEST(clientGetMinArea(NULL) == NO_CLIENT_VAL);
	ASSERT_TEST(clientGetMinRooms(NULL) == NO_CLIENT_VAL);
	ASSERT_TEST(clientGetMaxPrice(NULL) == NO_CLIENT_VAL);
	ASSERT_TEST(clientGetTotalPayments(NULL) == NO_CLIENT_VAL);
	ASSERT_TEST(emailAreEqual(clientGetMail(client), good_mail));
	ASSERT_TEST(clientGetMinArea(client) == 5);
	ASSERT_TEST(clientGetMinRooms(client) == 6);
	ASSERT_TEST(clientGetMaxPrice(client) == 7);
	ASSERT_TEST(clientGetTotalPayments(client) == 0);
	clientAddPayment(client, 50);
	ASSERT_TEST(clientGetTotalPayments(client) == 50);
	clientDestroy(client);
	emailDestroy(good_mail);
	return true;
}

/**
 * Test copy method
 */
static bool testClientCopy() {
	char* good_mail = "baba@ganosh";
	Client client, copy = NULL;
	Email mail;
	emailCreate(good_mail, &mail);
	clientCreate(mail,5, 5, 5, &client);
	clientAddPayment(client, 50);
	ASSERT_TEST(clientCopy(NULL, NULL) == CLIENT_NULL_PARAMETERS);
	ASSERT_TEST(clientCopy(client, NULL) == CLIENT_NULL_PARAMETERS);
	ASSERT_TEST(clientCopy(NULL, &copy) == CLIENT_NULL_PARAMETERS);
	ASSERT_TEST(clientCopy(client, &copy) == CLIENT_SUCCESS);
	ASSERT_TEST(emailAreEqual(clientGetMail(client), clientGetMail(copy)));
	ASSERT_TEST(clientGetMinArea(client) == clientGetMinArea(copy));
	ASSERT_TEST(clientGetMinRooms(client) == clientGetMinArea(copy));
	ASSERT_TEST(clientGetMaxPrice(client) == clientGetMinArea(copy));
	ASSERT_TEST(
		clientGetTotalPayments(client) == clientGetTotalPayments(copy));
	clientDestroy(copy);
	clientDestroy(client);
	emailDestroy(mail);
	return true;
}
