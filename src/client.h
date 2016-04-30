/*
 * Client.h
 *
 *  Created on: 30 באפר׳ 2016
 *      Author: binder
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

typedef struct ListItem_t *Client;

struct Cliet_t {
	char* Email;
	int Apartment_Min_Area;
	int Apartment_Min_Rooms;
	int Apartment_Max_Price;
	int Total_Mony_Paid;
};


#endif /* SRC_CLIENT_H_ */

Client ClientCreate();
