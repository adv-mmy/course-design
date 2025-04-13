#ifndef SYSTEM_INITIAL_SHUTDOWN_H
#define SYSTEM_INITIAL_SHUTDOWN_H

#include<stdio.h>
#include<stdlib.h>
#include"common_value.h"
#include"data_storage.h"
#include"inventory_management.h"
#include"fileio.h"

//��ʼ��ϵͳ
UserData* initUsers();
PackageData* initPackages();

//�ر�ϵͳ
void freeUsers(UserData** userList);
void freePackages(PackageData** packageList);
void freeInventory(InventoryManagement* inventory);

#endif