// inventory.c 库存管理模块
#include"inventory_management.h"

//初始化货架管理链表
InventoryManagement* createInventorySystem() {
    // 动态分配 InventoryManagement 结构体
    InventoryManagement* inventory = (InventoryManagement*)malloc(sizeof(InventoryManagement));
    if (!inventory) {
        perror("内存分配失败");
        return NULL;
    }

    inventory->totalShelves = 50;
    inventory->warningThreshold = 0.2;
    inventory->shelves = NULL;

    ShelfNode* prev = NULL;
    for (int level = 1; level <= 50; level++) {
        ShelfNode* newNode = (ShelfNode*)malloc(sizeof(ShelfNode));
        if (!newNode) {
            perror("内存分配失败");
            // 清理已分配的内存
            while (inventory->shelves != NULL) {
                ShelfNode* temp = inventory->shelves;
                inventory->shelves = inventory->shelves->nextShelfNode;
                free(temp);
            }
            free(inventory);
            return NULL;
        }

        // 初始化节点字段
        newNode->level = level;
        newNode->remainSpace = 2.0f; // 货架最大容量为2.0
        newNode->packagesOfThisLevel = NULL; // 初始化为空包裹列表
        newNode->nextShelfNode = NULL;
        newNode->packageCnt = 0; // 初始化包裹数量 
        
        // 链接到链表
        if (prev == NULL) {
            inventory->shelves = newNode;
        } else {
            prev->nextShelfNode = newNode;
        }
        prev = newNode;
    }

    return inventory;
}

// 最佳适应算法实现
ShelfNode* findBestFit(InventoryManagement* inv, float volume) {
    ShelfNode* best = NULL;
    ShelfNode* p = inv->shelves;
    float minRemaining = 2.0f; // 货架最大容量

    while (p != NULL) {
        if (p->remainSpace >= volume && p->remainSpace < minRemaining) {
            best = p;
            minRemaining = p->remainSpace;
        }
        p = p->nextShelfNode;
    }
    return best;
}

// 入库时的空间分配
bool allocateShelf(InventoryManagement* inv, PackageData* parcel) {
    ShelfNode* targetShelf = findBestFit(inv, parcel->volume);
    
    if (targetShelf != NULL) {
        targetShelf->remainSpace -= parcel->volume;
        generatePickupCode(parcel, targetShelf->level);
        return true;
    }

    return false; // 空间不足
}


// 包裹入库接口
void addParcelToInventory(InventoryManagement* inv, PackageData* parcel) {
    if (parcel->volume > 2.0f) {
        printf("错误：包裹体积超过货架最大容量\n");
        return;
    }

    bool result = allocateShelf(inv, parcel);
    if (!result) {
        printf("很抱歉，仓库已满，暂停寄件\n");
    }
    return;
}