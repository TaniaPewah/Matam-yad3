#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "email.h"
#include "test_utilities.h"

static bool testOfferCreate();
static bool testOfferCopy();
static bool testOfferCompareAndEqals();
static bool testOfferGeters();

//int main() {
int RunOfferTest() {
	RUN_TEST(testOfferCreate);
	RUN_TEST(testOfferCopy);
	RUN_TEST(testOfferCompareAndEqals);
	RUN_TEST(testOfferGeters);
	return 0;
}

/**
 * Test create method
 */
static bool testOfferCreate() {
	char* good_mail = "baba@ganosh";
	char* bad_mail_1  = "baba+ganosh";
	char* bad_mail_2  = "";
	char* bad_mail_3  = "lala lala";
	Email mail;
	ASSERT_TEST(emailCreate(NULL, NULL) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCreate(NULL, &mail) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCreate(good_mail, NULL) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCreate(bad_mail_1, &mail) == EMAIL_INVALID_PARAMETERS);
	ASSERT_TEST(emailCreate(bad_mail_2, &mail) == EMAIL_INVALID_PARAMETERS);
	ASSERT_TEST(emailCreate(bad_mail_3, &mail) == EMAIL_INVALID_PARAMETERS);
	ASSERT_TEST(emailCreate(good_mail, &mail) == EMAIL_SUCCESS);
	ASSERT_TEST(mail != NULL);
	emailDestroy(mail);
	return true;
}

/**
 * Test copy method
 */
static bool testOfferCopy() {
	char* good_mail = "awsome@mail.com";
	Email mail, copy = NULL;
	emailCreate(good_mail, &mail);
	ASSERT_TEST(emailCopy(NULL, NULL) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCopy(mail, NULL) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCopy(NULL, &copy) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCopy(mail, &copy) == EMAIL_SUCCESS);
	ASSERT_TEST(emailAreEqual(mail, copy));
	emailDestroy(copy);
	emailDestroy(mail);
	return true;
}

/**
 * Test Compare method and Equal method
 */
static bool testOfferCompareAndEqals() {
	char* address_1  = "aaa@gmail.com";
	char* address_2  = "aaa@gmail.com";
	char* address_3  = "bbb@gmail.com";
	char* address_4  = "Bbb@gmail.com";

	Email mail_1, mail_2, mail_3, mail_4;
	ASSERT_TEST(emailCreate(address_1, &mail_1) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(address_2, &mail_2) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(address_3, &mail_3) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(address_4, &mail_4) == EMAIL_SUCCESS);

	ASSERT_TEST(emailAreEqual(NULL, NULL) == true);
	ASSERT_TEST(emailAreEqual(mail_1, NULL) == false);
	ASSERT_TEST(emailAreEqual(NULL, mail_1) == false);
	ASSERT_TEST(emailAreEqual(mail_1, mail_2) == true);
	ASSERT_TEST(emailAreEqual(mail_1, mail_1) == true);
	ASSERT_TEST(emailAreEqual(mail_1, mail_3) == false);
	ASSERT_TEST(emailAreEqual(mail_3, mail_1) == false);

	ASSERT_TEST(emailComapre(NULL, NULL) == 0);
	ASSERT_TEST(emailComapre(mail_3, NULL) > 0);
	ASSERT_TEST(emailComapre(mail_3, mail_1) > 0);
	ASSERT_TEST(emailComapre(NULL, mail_1) < 0);
	ASSERT_TEST(emailComapre(mail_1, mail_2) == 0);
	ASSERT_TEST(emailComapre(mail_2, mail_1) == 0);
	ASSERT_TEST(emailComapre(mail_3, mail_4) > 0);
	ASSERT_TEST(emailComapre(mail_4, mail_3) < 0);

	emailDestroy(mail_1);
	emailDestroy(mail_2);
	emailDestroy(mail_3);
	emailDestroy(mail_4);
	return true;
}

/**
 * Test method emailToString
 */
static bool testOfferGeters() {
	char* address_1  = "aaa@gmail.com";
	char* address_2  = "bbb@gmail.com";
	Email mail_1 = NULL;
	Email mail_2 = NULL;
	char *new_address_1 = NULL;
	char *new_address_2 = NULL;
	ASSERT_TEST(emailCreate(address_1, &mail_1) == EMAIL_SUCCESS);
	ASSERT_TEST(emailCreate(address_2, &mail_2) == EMAIL_SUCCESS);
	ASSERT_TEST(emailToString(NULL) == NULL);
	new_address_1 = emailToString(mail_1);
	new_address_2 = emailToString(mail_2);
	ASSERT_TEST(new_address_1 != NULL);
	ASSERT_TEST(new_address_2 != NULL);
	ASSERT_TEST(strcmp(address_1, new_address_1) == 0);
	ASSERT_TEST(strcmp(address_2, new_address_2) == 0);
	ASSERT_TEST(strcmp(address_2, new_address_1) != 0);
	ASSERT_TEST(strcmp(address_1, new_address_2) != 0);
	free(new_address_1);
	free(new_address_2);
	emailDestroy(mail_1);
	emailDestroy(mail_2);
	return true;
}
