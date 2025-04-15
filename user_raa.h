#ifndef USER_REGISTER_AND_AUTHENTICATE_H
#define USER_REGISTER_AND_AUTHENTICATE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"data_storage.h"
#include"str_and_enum.h"
#include"common_value.h"

void userRegisterFlow(UserData** userList);
UserData* authenticateUser(UserData* userList, const char* account, const char* password);

#endif