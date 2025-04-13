#ifndef INVENTORY_MANAGEMENT_H
#define INVENTORY_MANAGEMENT_H

#include "common_value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <float.h>

//��������
typedef struct shelfNode{
  PackageData* packagesOfThisLevel;
  int level;
  int packageCnt; 
  float remainSpace; 
  struct shelfNode* nextShelfNode;        
}ShelfNode;

/*���ܹ�������ṹ��*/
typedef struct InventoryManagement {
  ShelfNode* shelves;       // ��������ͷָ��
  int totalShelves;         // �ܻ��������̶�Ϊ50�㣩
  float warningThreshold;   // ���ܿռ�Ԥ����ֵ��ʣ��10%ʱԤ����
} InventoryManagement;

InventoryManagement* createInventorySystem();
ShelfNode* findBestShelf(InventoryManagement* inv, float volume);
bool allocateShelf(InventoryManagement* inv, PackageData* parcel);
void addParcelToInventory(InventoryManagement* inv, PackageData* parcel);
void generatePickupCode(PackageData* parcel, ShelfNode* shelf);

#endif