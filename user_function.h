#ifndef USER_FUNCTION_H
#define USER_FUNCTION_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include"common_value.h"
#include"data_storage.h"
#include"inventory_management.h"

void sendParcel(PackageData** packageList, UserData* user, InventoryManagement* inventory);
void searchParcelInterface(UserData* user, PackageData* packageList);
void getParcelFromInventory(UserData* user, PackageData* packageList, InventoryManagement* inventory);
void cancelSending(UserData* user, PackageData** packageList, InventoryManagement* inventory);
void displayUserHistory(UserData* user, PackageData* packageList);
void modifyUserProfile(UserData* user);
void deleteUser(UserData** userList, const char* userName);

#endif