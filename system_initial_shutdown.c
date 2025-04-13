#include"system_initial_shutdown.h"

//��ʼ���û���Ϣ�洢����
UserData* initUsers(){
  FILE* fp=fopen("user_data.txt", "a+");
    if (fp == NULL) {
        perror("�û���Ϣ��ʼ��ʧ�ܣ�"); 
        exit(0);
    }
    UserData* head = NULL;      // ����ͷ�ڵ�
    UserData* current = NULL;   // ��ǰ�ڵ�

    while (!feof(fp)) {
        UserData* newNode = (UserData*)malloc(sizeof(UserData));
        if (!newNode) {
            perror("�ڴ����ʧ��");
            fclose(fp);
            return head;
        }

        if (ReadUserFromFile(fp, newNode)) {
            newNode->nextUserData = NULL;

            if (head == NULL) {
                // ��һ���ڵ�
                head = newNode;
                current = head;
            } else {
                // ���ӵ�����β��
                current->nextUserData = newNode;
                current = current->nextUserData;
            }
        } else {
            // ��ȡʧ��ʱ�ͷ���Ч�ڵ�
            free(newNode);
            continue;
        }
    }

    fclose(fp);
    return head;
}

//��ʼ��������Ϣ�洢����
PackageData* initPackages(){
  FILE* fp=fopen("package_data.txt", "a+");
    if (fp == NULL) {
        perror("������Ϣ��ʼ��ʧ�ܣ�"); 
        exit(0);
    }
    PackageData* head = NULL;
    PackageData* current = NULL;

    while (!feof(fp)) {
        PackageData* newNode = (PackageData*)malloc(sizeof(PackageData));
        if (!newNode) {
            perror("�ڴ����ʧ��");
            fclose(fp);
            return head;
        }

        if (ReadPackageFromFile(fp, newNode)) {
            newNode->nextPackageData = NULL;

            if (head == NULL) {
                head = newNode;
                current = head;
            } else {
                current->nextPackageData = newNode;
                current = current->nextPackageData;
            }
        } else {
            free(newNode);
            continue;
        }
    }

    fclose(fp);
    return head;
}

//�ͷ��û���Ϣ�洢�ռ�
void freeUsers(UserData** userList) {
  UserData* current = *userList;
  while (current != NULL) {
      UserData* temp = current;
      current = current->nextUserData;
      free(temp);
  }
  *userList = NULL; // �ÿ��ⲿͷָ��
}

//�ͷŰ�����Ϣ�洢�ռ�
void freePackages(PackageData** packageList) {
  PackageData* current = *packageList;
  while (current != NULL) {
      PackageData* temp = current;
      current = current->nextPackageData;
      free(temp);
  }
  *packageList = NULL; // �ÿ��ⲿͷָ��
}

//�ͷŻ��ܹ�����Ϣ�洢�ռ�
void freeInventory(InventoryManagement* inventory){
  ShelfNode* tmpPtr=inventory->shelves, *freePtr=inventory->shelves;
  while(freePtr!=NULL){
    tmpPtr=tmpPtr->nextShelfNode;
    free(freePtr);
    freePtr=tmpPtr;
  }
  free(inventory);
  return;
}