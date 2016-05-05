#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "email.h"

int RunEmailTest();
static bool testEmailCreate();
static bool testEmailCopy();

//int main() {
int RunEmailTest() {
	RUN_TEST(testEmailCreate);
	RUN_TEST(testEmailCopy);
	return 0;
}

/**
 * Test create method
 */
static bool testEmailCreate() {
	char* good_mail = "baba@ganosh";
	char* bad_mail_1  = "baba+ganosh";
	char* bad_mail_2  = "";
	//char* bad_mail_3  = "lala lala";
	Email mail;
	ASSERT_TEST(emailCreate(NULL, NULL) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCreate(NULL, &mail) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCreate(good_mail, NULL) == EMAIL_NULL_PARAMETERS);
	ASSERT_TEST(emailCreate(bad_mail_1, &mail) == EMAIL_INVALID_PARAMETERS);
	ASSERT_TEST(emailCreate(bad_mail_2, &mail) == EMAIL_INVALID_PARAMETERS);
	ASSERT_TEST(emailCreate(good_mail, &mail) == EMAIL_SUCCESS);
	ASSERT_TEST(mail != NULL);
	emailDestroy(mail);
	return true;
}

/**
 * Test copy method
 */
static bool testEmailCopy() {
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
