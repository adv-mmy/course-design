#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common_value.h"
#include "data_storage.h"
#include "inventory_management.h"
#include "fileio.h"
#include "str_and_enum.h"
#include "user_function.h"
#include "admin_function.h"
#include "system_initial_shutdown.h"
#include "User_raa.h"

void userLoginFlow();
void userDashboard(UserData*);
void adminDashboard(UserData*);

// ȫ��ϵͳ״̬
typedef struct{
    UserData* userList;
    PackageData* packageList;
    InventoryManagement* inventoryA; //�ֳ��������ܣ�A��Ŵ�ȡ���B��Ŵ��Ļ����״̬�������
    InventoryManagement* inventoryB;
    int isRunning;
}systemState;

systemState appSystem;
/*�ļ��״�����ʱ�����а�����ȡ���������Ϊ00-000����ȡ�����ڽ�����ܺ��ٽ��з���ȡ����*/
// ϵͳ��ʼ��
void initSystem() {
    FILE* inventoryAptr=fopen("inventoryA.txt", "a+");
    FILE* inventoryBptr=fopen("inventoryB.txt", "a+");
    appSystem.isRunning = 1;
    appSystem.userList=initUsers();
    appSystem.packageList=initPackages();
    appSystem.inventoryA=createInventorySystem(inventoryAptr);
    appSystem.inventoryB=createInventorySystem(inventoryBptr);
    fclose(inventoryAptr);
    fclose(inventoryBptr);
    //��ʼ��ϵͳʱ��ȫ�������������
    PackageData* enterInventoryPtr=appSystem.packageList;
    while(enterInventoryPtr!=NULL){
        if(enterInventoryPtr->packageStatus == pendingPickup || enterInventoryPtr->packageStatus == pickedUp){
            addParcelToInventory(appSystem.inventoryA, enterInventoryPtr);
            enterInventoryPtr=enterInventoryPtr->nextPackageData;
        }else{
            addParcelToInventory(appSystem.inventoryB, enterInventoryPtr);
            enterInventoryPtr=enterInventoryPtr->nextPackageData;
        }
    }
}

// ϵͳ�ر�
void shutdownSystem() {
    FILE* userDataFilePtr=fopen("user_data.txt", "w+");
    FILE* packageDataFilePtr=fopen("package_data.txt", "w+");
    FILE* inventoryAptr=fopen("inventoryA.txt", "w+");
    FILE* inventoryBptr=fopen("inventoryB.txt", "w+");
    //д���ļ�ʱ���õ�ָ��
    UserData* curUser=appSystem.userList;
    PackageData* curPackage=appSystem.packageList;
    ShelfNode* curShelfOfA=appSystem.inventoryA->shelves;
    ShelfNode* curShelfOfB=appSystem.inventoryB->shelves;
    while(curUser!=NULL){
        WriteUserToFile(userDataFilePtr, curUser);
        curUser=curUser->nextUserData;
    }
    while(curPackage!=NULL){
        WritePackageToFile(packageDataFilePtr, curPackage);
        curPackage=curPackage->nextPackageData;
    }
    while(curShelfOfA!=NULL){
        WriteInventoryStatusToFile(inventoryAptr, curShelfOfA);
        curShelfOfA=curShelfOfA->nextShelfNode;
    }
    while(curShelfOfB!=NULL){
        WriteInventoryStatusToFile(inventoryBptr, curShelfOfB);
        curShelfOfB=curShelfOfB->nextShelfNode;
    }
    fclose(userDataFilePtr);
    fclose(packageDataFilePtr);
    freeUsers(&appSystem.userList);
    freePackages(&appSystem.packageList);
    freeInventory(appSystem.inventoryA);
    freeInventory(appSystem.inventoryB);
}

// ����������
void mainMenu() {
    while (appSystem.isRunning) {
        printf("\n=== ������վ���˵� ===\n");
        printf("1. �û���¼\n");
        printf("2. �û�ע��\n");
        printf("3. �˳�ϵͳ\n");
        printf("��ѡ�����: ");

        int choice;
        scanf("%d", &choice);
        while(getchar() != '\n'); // ������뻺����

        switch (choice) {
            case 1: userLoginFlow(); break;
            case 2: userRegisterFlow(&appSystem.userList); break;
            case 3: appSystem.isRunning = 0; break;
            default: 
                printf("��Ч���룬������ѡ��\n");
                while(getchar()!='\n');
        }
        system("cls");
    }
}

// �û���¼����
void userLoginFlow() {
    char account[UserNameLen], password[PinLen];
    printf("�������û���: \n");
    fgets(account, UserNameLen, stdin);
    account[strcspn(account, "\n")]='\0';    // �����з�
    printf("����������: \n");
    fgets(password, PinLen, stdin);
    password[strcspn(password, "\n")]='\0';   //�����з�

    UserData* user = authenticateUser(appSystem.userList, account, password);
    if (user!=NULL) {
        if (user->permission) {
            adminDashboard(user);
        } else {
            userDashboard(user);
        }
    } else {
        printf("�˺Ż��������");
    }
    getchar();
    return;
}

// �û��������
void userDashboard(UserData* user) {
    while (appSystem.isRunning) {
        printf("\n=== �û���� [%s] ===\n", user->userName);
        searchParcelInterface(user, appSystem.packageList);        //�鿴���޴�ȡ����
        printf("\n");
        printf("������\n");
        printf("1. ȡ��\n");
        printf("2. ���Ͱ���\n");
        printf("3. ȡ���ļ�\n");
        printf("4. �鿴��ʷ����\n");
        printf("5. �޸ĸ�����Ϣ\n");   
        printf("6. ע���˺�\n");
        printf("7. �������˵�\n");
        printf("8. �˳�ϵͳ\n");
        printf("��ѡ�����: \n");

        int choice;
        scanf("%d", &choice);
        while(getchar() != '\n');
        system("cls");
        switch (choice) {
            case 1: getParcelFromInventory(user, appSystem.packageList, appSystem.inventoryA); break;     //ȡ��
            case 2: createParcel(&appSystem.packageList, user, appSystem.inventoryB); break;
            case 3: cancelSending(user, &appSystem.packageList, appSystem.inventoryB); break; 
            case 4: displayUserHistory(user, appSystem.packageList); break;
            case 5: modifyUserProfile(user); break;
            case 6: 
                deleteUser(&appSystem.userList, user->userName);
                return;
            case 7: return;
            case 8: appSystem.isRunning = 0; break;
            default: {
                printf("��Ч����������������ѡ�\n");
                while(getchar()!='\n');
            }
        }
        system("cls");
    }
}

// ����Ա���
void adminDashboard(UserData* admin) {
    while (appSystem.isRunning) {
        printf("��ǰ����Ա��");
        fputs(admin->name, stdout);
        printf("\n");
        printf("\n=== �������̨ ===\n");
        printf("1. �鿴���״̬\n");           //��˳���ӡ�����ܵ�ǰʣ��ռ�                                  
        printf("2. �����ĳ��Ǽ�\n");
        printf("3. ���������Ǽ�\n");
        printf("4. �������˵�\n");
        printf("5. �˳�ϵͳ\n");
        printf("��ѡ�����: \n");

        int choice;
        scanf("%d", &choice);
        while(getchar() != '\n');
        system("cls");
        switch (choice) {
            case 1: {
                printf("��ȡ����\n");
                displayInventoryStatus(appSystem.inventoryA);
                printf("\n��������\n");
                displayInventoryStatus(appSystem.inventoryB);
                break;
            }
            case 2: sentParcelRecording(appSystem.packageList, appSystem.inventoryB); break;
            case 3: addNewParcelToList(&appSystem.packageList, appSystem.inventoryA); break;    //���ȡ�������°���
            case 4: return;
            case 5: appSystem.isRunning = 0; break;
            default: {
                printf("��Ч����������������ѡ�\n");
                while(getchar()!='\n');
            }
        }
        system("cls");
    }
}

int main() {
    initSystem();
    mainMenu();
    shutdownSystem();
    printf("��Ϣ�洢���,�����س����˳�����\n");
    while(getchar()=='\n')return 0;
}