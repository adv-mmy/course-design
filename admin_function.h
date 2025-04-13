#ifndef ADMIN_FUNCTION_H
#define ADMIN_FUNCTION_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"common_value.h"
#include"inventory_management.h"

void displayInventoryStatus(InventoryManagement* inventory);
void sentParcelRecording(PackageData* packageList, InventoryManagement* inventory);
void addNewParcelToList(PackageData** packageList, InventoryManagement* inventory);

#endif