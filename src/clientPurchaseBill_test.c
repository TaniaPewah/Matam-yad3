#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "email.h"
#include "utilities.h"
#include "clientPurchaseBill.h"
#include "test_utilities.h"

static bool testClientPurchaseBillCreate();
static bool testClientPurchaseBillCopy();
static bool testClientPurchaseBillCompare();
static bool testClientPurchaseBillGeters();

//int main() {
int RunClientPurchaseBillTest() {
	RUN_TEST(testClientPurchaseBillCreate);
	RUN_TEST(testClientPurchaseBillCopy);
	RUN_TEST(testClientPurchaseBillCompare);
	RUN_TEST(testClientPurchaseBillGeters);
	return 0;
}

/**
 * Test create method
 */
static bool testClientPurchaseBillCreate() {
	char* address = "lala.hkoala@walla.com";
	Email mail;
	ASSERT_TEST(emailCreate(address, &mail) == EMAIL_SUCCESS);
	ClientPurchaseBill bill = clientPurchaseBillCreate(NULL, 6500);
	ASSERT_TEST(bill == NULL);
	bill = clientPurchaseBillCreate(mail, -5);
	ASSERT_TEST(bill == NULL);
	bill = clientPurchaseBillCreate(mail, 6500);
	ASSERT_TEST(bill != NULL);
	clientPurchaseBillDestroy(bill);
	emailDestroy(mail);
	return true;
}

/**
 * Test copy method
 */
static bool testClientPurchaseBillCopy() {
	char* good_mail = "my_stupid_test@nana10.com";
	char* good_mail_2 = "banana@levana.com";
	Email mail;
	emailCreate(good_mail, &mail);
	Email mail_2;
	emailCreate(good_mail_2, &mail_2);
	ASSERT_TEST(mail != NULL);
	ClientPurchaseBill bill = clientPurchaseBillCreate(mail, 7500);
	ASSERT_TEST(bill != NULL);
	ClientPurchaseBill bill_second = clientPurchaseBillCreate(mail_2, 4300);
	ASSERT_TEST(bill != NULL);
	ClientPurchaseBill copy = clientPurchaseBillCopy(NULL);
	ASSERT_TEST(copy == NULL);
	copy = clientPurchaseBillCopy(bill);
	ASSERT_TEST(copy != NULL);
	ASSERT_TEST(clientPurchaseBillComapre(bill, copy) == 0);
	ASSERT_TEST(clientPurchaseBillComapre(bill_second, copy) != 0);
	clientPurchaseBillDestroy(bill);
	clientPurchaseBillDestroy(bill_second);
	clientPurchaseBillDestroy(copy);
	emailDestroy(mail);
	return true;
}

/**
 * Test Compare method and Equal method
 */
static bool testClientPurchaseBillCompare() {
	char* good_mail_1 = "aaa@nana10.com";
	char* good_mail_2 = "bbb@levana.com";
	char* good_mail_3 = "ccc@levana.com";
	Email mail_1, mail_2, mail_3;
	emailCreate(good_mail_1, &mail_1);
	emailCreate(good_mail_2, &mail_2);
	emailCreate(good_mail_3, &mail_3);
	ASSERT_TEST(mail_1 != NULL);
	ASSERT_TEST(mail_2 != NULL);
	ASSERT_TEST(mail_3 != NULL);
	ClientPurchaseBill bill_1 = clientPurchaseBillCreate(mail_1, 1000);
	ClientPurchaseBill bill_2 = clientPurchaseBillCreate(mail_2, 500);
	ClientPurchaseBill bill_3 = clientPurchaseBillCreate(mail_3, 7500);
	ASSERT_TEST(bill_1 != NULL);
	ASSERT_TEST(bill_2 != NULL);
	ASSERT_TEST(bill_3 != NULL);
	ASSERT_TEST(clientPurchaseBillComapre(bill_1, bill_1) == 0);
	ASSERT_TEST(clientPurchaseBillComapre(NULL, NULL) == 0);
	ASSERT_TEST(clientPurchaseBillComapre(bill_1, NULL) < 0);
	ASSERT_TEST(clientPurchaseBillComapre(NULL, bill_1) > 0);

	ASSERT_TEST(clientPurchaseBillComapre(bill_1, bill_3) > 0);
	ASSERT_TEST(clientPurchaseBillComapre(bill_2, bill_3) > 0);
	ASSERT_TEST(clientPurchaseBillComapre(bill_3, bill_1) < 0);
	ASSERT_TEST(clientPurchaseBillComapre(bill_3, bill_2) < 0);

	clientPurchaseBillDestroy(bill_1);
	clientPurchaseBillDestroy(bill_2);
	clientPurchaseBillDestroy(bill_3);
	emailDestroy(mail_1);
	emailDestroy(mail_2);
	emailDestroy(mail_3);
	return true;
}

/**
 * Test method emailToString
 */
static bool testClientPurchaseBillGeters() {
	char* good_mail_1 = "shama@blalba.com";
	char* good_mail_2 = "abc@xyz.com";
	Email mail_1, mail_2;
	emailCreate(good_mail_1, &mail_1);
	emailCreate(good_mail_2, &mail_2);
	ASSERT_TEST(mail_1 != NULL);
	ASSERT_TEST(mail_2 != NULL);
	ClientPurchaseBill bill_1 = clientPurchaseBillCreate(mail_1, 1000);
	ClientPurchaseBill bill_2 = clientPurchaseBillCreate(mail_2, 500);
	ASSERT_TEST(bill_1 != NULL);
	ASSERT_TEST(bill_2 != NULL);
	ASSERT_TEST(clientPurchaseBillGetClientEmailToString(NULL) == NULL);
	char* address = clientPurchaseBillGetClientEmailToString(bill_1);
	ASSERT_TEST(areStringsEqual(address, good_mail_1) == true);
	ASSERT_TEST(clientPurchaseBillGetMoneyPaidString(NULL) == NULL);
	char* paid = clientPurchaseBillGetMoneyPaidString(bill_2);
	ASSERT_TEST(areStringsEqual(paid, "500") == true);
	free(address);
	free(paid);
	clientPurchaseBillDestroy(bill_1);
	clientPurchaseBillDestroy(bill_2);
	emailDestroy(mail_1);
	emailDestroy(mail_2);
	return true;
}
