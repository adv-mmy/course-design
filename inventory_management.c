// inventory.c ������ģ��
#include"inventory_management.h"

//��ʼ�����ܹ�������
InventoryManagement* createInventorySystem() {
    // ��̬���� InventoryManagement �ṹ��
    InventoryManagement* inventory = (InventoryManagement*)malloc(sizeof(InventoryManagement));
    if (!inventory) {
        perror("�ڴ����ʧ��");
        return NULL;
    }

    inventory->totalShelves = 50;
    inventory->warningThreshold = 0.2f;
    inventory->shelves = NULL;

    ShelfNode* prev = NULL;
    for (int level = 1; level <= 50; level++) {
        ShelfNode* newNode = (ShelfNode*)malloc(sizeof(ShelfNode));
        if (!newNode) {
            perror("�ڴ����ʧ��");
            // �����ѷ�����ڴ�
            while (inventory->shelves != NULL) {
                ShelfNode* temp = inventory->shelves;
                inventory->shelves = inventory->shelves->nextShelfNode;
                free(temp);
            }
            free(inventory);
            return NULL;
        }

        // ��ʼ���ڵ��ֶ�
        newNode->level = level;
        newNode->remainSpace = 2.0f; // �����������Ϊ2.0
        newNode->packagesOfThisLevel = NULL; // ��ʼ��Ϊ�հ����б�
        newNode->nextShelfNode = NULL;
        newNode->packageCnt = 0; // ��ʼ���������� 
        
        // ���ӵ�����
        if (prev == NULL) {
            inventory->shelves = newNode;
        } else {
            prev->nextShelfNode = newNode;
        }
        prev = newNode;
    }

    return inventory;
}

// �����Ӧ�㷨ʵ��
ShelfNode* findBestShelf(InventoryManagement* inventory, float volume) {
    ShelfNode* bestShelf = NULL;
    ShelfNode* curShelf = inventory->shelves;
    float minRemaining = FLT_MAX; // ��ʼ��Ϊ����ֵ

    while (curShelf != NULL) {
        // ʣ��ռ����㹻���ɰ������Ҿ�����С
        if (curShelf->remainSpace >= volume && curShelf->remainSpace < minRemaining) {
            bestShelf = curShelf;
            minRemaining = curShelf->remainSpace;
        }
        curShelf = curShelf->nextShelfNode;
    }
    return bestShelf;
}

// ���ʱ�Ŀռ����
bool allocateShelf(InventoryManagement* inventory, PackageData* parcel) {
    ShelfNode* targetShelf = findBestShelf(inventory, parcel->volume);
    
    if (targetShelf != NULL) {
        // 1. ���»���ʣ��ռ�
        targetShelf->remainSpace -= parcel->volume;

        // 2. ���»��ܰ�������
        targetShelf->packageCnt++;
        
        // 3. ����ȡ���벢��������
        generatePickupCode(parcel, targetShelf); // ���� generatePickupCode ��Ҫ���ܲ㼶

        // 4. ��������ӵ����ܵİ�������
        parcel->nextPackageData = targetShelf->packagesOfThisLevel;
        targetShelf->packagesOfThisLevel = parcel;

        return true;
    }
    return false; // �ռ䲻��
}


// �������ӿ�
void addParcelToInventory(InventoryManagement* inventory, PackageData* parcel) {
    //�����ȡ���루���ǵ�һ����⣩������
    if(strcmp(parcel->pickUpCode, "0-00-000")!=0){
        return;
    }
    if (parcel->volume > 2.0f) {
        printf("���󣺰�����������������������2.0m?��\n");
        return;
    }

    bool result = allocateShelf(inventory, parcel);
    if (!result) {
        printf("���棺�ֿ�������\n");
    }
}
//����ȡ����
void generatePickupCode(PackageData* parcel, ShelfNode* shelf) {
    // ʾ����ȡ�����ʽΪ "A-01-001"����������-�㼶-��ţ�+
    if(parcel->packageStatus == pendingPickup || parcel->packageStatus == pickedUp){
        sprintf(parcel->pickUpCode, "A-%02d-%03d", shelf->level, shelf->packageCnt);
    }else{
        sprintf(parcel->pickUpCode, "B-%02d-%03d", shelf->level, shelf->packageCnt);
    }
}