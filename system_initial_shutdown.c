#include<stdio.h>
#include<stdlib.h>
#include"common_value.h"
#include"data_storage.h"
#include"inventory_management.h"
#include"fileio.h"
#include"system_initial_shutdown.h"

//初始化用户信息存储链表
UserData* initUsers(){
  FILE* fp=fopen("user_user_data.txt", "r");
    if (fp == NULL) {
        perror("用户信息初始化失败！"); 
        exit(0);
    }
    UserData* head = NULL;      // 链表头节点
    UserData* current = NULL;   // 当前节点

    while (!feof(fp)) {
        UserData* newNode = (UserData*)malloc(sizeof(UserData));
        if (!newNode) {
            perror("内存分配失败");
            fclose(fp);
            return head;
        }

        if (ReadUserFromFile(fp, newNode)) {
            newNode->nextUserData = NULL;

            if (head == NULL) {
                // 第一个节点
                head = newNode;
                current = head;
            } else {
                // 链接到链表尾部
                current->nextUserData = newNode;
                current = current->nextUserData;
            }
        } else {
            // 读取失败时释放无效节点
            free(newNode);
            continue;
        }
    }

    fclose(fp);
    return head;
}

//初始化包裹信息存储链表
PackageData* initPackages(){
  FILE* fp=fopen("package_data.txt", "r");
    if (fp == NULL) {
        perror("包裹信息初始化失败！"); 
        exit(0);
    }
    PackageData* head = NULL;
    PackageData* current = NULL;

    while (!feof(fp)) {
        PackageData* newNode = (PackageData*)malloc(sizeof(PackageData));
        if (!newNode) {
            perror("内存分配失败");
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

//释放用户信息存储空间
void freeUsers(UserData** userList) {
  UserData* current = *userList;
  while (current != NULL) {
      UserData* temp = current;
      current = current->nextUserData;
      free(temp);
  }
  *userList = NULL; // 置空外部头指针
}

//释放包裹信息存储空间
void freePackages(PackageData** packageList) {
  UserData* current = *packageList;
  while (current != NULL) {
      UserData* temp = current;
      current = current->nextPackageData;
      free(temp);
  }
  *packageList = NULL; // 置空外部头指针
}

//释放货架管理信息存储空间
void freeInventory(InventoryManagement* inventory){
  ShelfNode* tmpPtr=inventory->shelves, *freePtr=inventory->shelves;
  while(freePtr!=NULL){
    tmpPtr=inventory->shelves->nextShelfNode;
    free(freePtr);
    freePtr=tmpPtr;
  }
  free(inventory);
  return;
}