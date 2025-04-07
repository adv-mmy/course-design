#ifndef INVENTORY_MANAGEMENT_H
#define INVENTORY_MANAGEMENT_H

#include "common_value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <float.h>

//货架链表
typedef struct shelfNode{
  PackageData* packagesOfThisLevel;
  int level;
  int packageCnt; 
  float remainSpace; 
  struct shelfNode* nextShelfNode;        
}ShelfNode;

/*货架管理参数结构体*/
typedef struct InventoryManagement {
  ShelfNode* shelves;       // 货架链表头指针
  int totalShelves;         // 总货架数（固定为50层）
  float warningThreshold;   // 货架空间预警阈值（剩余10%时预警）
} InventoryManagement;

InventoryManagement* createInventorySystem();
ShelfNode* findBestShelf(InventoryManagement* inv, float volume);
bool allocateShelf(InventoryManagement* inv, PackageData* parcel);
void addParcelToInventory(InventoryManagement* inv, PackageData* parcel);
void generatePickupCode(PackageData* parcel, int shelfLevel);

#endif