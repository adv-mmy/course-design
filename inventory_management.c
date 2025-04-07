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
    inventory->warningThreshold = 0.2f;
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
ShelfNode* findBestShelf(InventoryManagement* inventory, float volume) {
    ShelfNode* bestShelf = NULL;
    ShelfNode* curShelf = inventory->shelves;
    float minRemaining = FLT_MAX; // 初始化为极大值

    while (curShelf != NULL) {
        // 剩余空间需足够容纳包裹，且尽可能小
        if (curShelf->remainSpace >= volume && curShelf->remainSpace < minRemaining) {
            bestShelf = curShelf;
            minRemaining = curShelf->remainSpace;
        }
        curShelf = curShelf->nextShelfNode;
    }
    return bestShelf;
}

// 入库时的空间分配
bool allocateShelf(InventoryManagement* inventory, PackageData* parcel) {
    ShelfNode* targetShelf = findBestShelf(inventory, parcel->volume);
    
    if (targetShelf != NULL) {
        // 1. 更新货架剩余空间
        targetShelf->remainSpace -= parcel->volume;

        // 2. 更新货架包裹计数
        targetShelf->packageCnt++;
        
        // 3. 生成取件码并关联货架
        generatePickupCode(parcel, targetShelf); // 假设 generatePickupCode 需要货架层级

        // 4. 将包裹添加到货架的包裹链表
        parcel->nextPackageData = targetShelf->packagesOfThisLevel;
        targetShelf->packagesOfThisLevel = parcel;

        return true;
    }
    return false; // 空间不足
}


// 包裹入库接口
void addParcelToInventory(InventoryManagement* inventory, PackageData* parcel) {
    //如果有取件码（不是第一次入库）就跳过
    if(strcmp(parcel->pickUpCode, "0-00-000")!=0){
        return;
    }
    if (parcel->volume > 2.0f) {
        printf("错误：包裹体积超过货架最大容量（2.0m?）\n");
        return;
    }

    bool result = allocateShelf(inventory, parcel);
    if (!result) {
        printf("警告：仓库已满！\n");
    }
}
//分配取件码
void generatePickupCode(PackageData* parcel, ShelfNode* shelf) {
    // 示例：取件码格式为 "A-01-001"（货架区号-层级-序号）+
    if(parcel->packageStatus == pendingPickup || parcel->packageStatus == pickedUp){
        sprintf(parcel->pickUpCode, "A-%02d-%03d", shelf->level, shelf->packageCnt);
    }else{
        sprintf(parcel->pickUpCode, "B-%02d-%03d", shelf->level, shelf->packageCnt);
    }
}