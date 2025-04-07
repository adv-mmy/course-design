#ifndef USER_FUNCTION_H
#define USER_FUNCTION_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include"common_value.h"

PackageData* createParcel(PackageData** packageList, UserData* user);
void searchParcelInterface(UserData* user, PackageData* packageList);
void getParcelFromInventory(UserData* user, PackageData* packageList);
void displayUserHistory(UserData* user, PackageData* packageList);
void modifyUserProfile(UserData* user);
void deleteUser(UserData** userList, const char* userName);

#endif