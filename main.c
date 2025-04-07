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

// 全局系统状态
typedef struct{
    UserData* userList;
    PackageData* packageList;
    InventoryManagement* inventory;
    int isRunning;
}systemState;

systemState appSystem;
/*文件首次载入时，所有包裹的取件码均设置为00-000，待取包裹在进入货架后再进行分配取件码*/
// 系统初始化
void initSystem() {
    appSystem.userList=initUsers();
    appSystem.packageList=initPackages();
    appSystem.inventory=createInventorySystem();
    //初始化系统时将全部包裹放入货架
    PackageData* enterInventoryPtr=appSystem.packageList;
    while(enterInventoryPtr->nextPackageData!=NULL){
        addParcelToInventory(appSystem.inventory, enterInventoryPtr);
        enterInventoryPtr=enterInventoryPtr->nextPackageData;
    }

    appSystem.isRunning = 1;
}

// 系统关闭
void shutdownSystem() {
    FILE* userDataFilePtr=fopen("user_data.txt", "w+");
    FILE* packageDataFilePtr=fopen("package_data.txt", "w+");
    WriteUserToFile(userDataFilePtr, appSystem.userList);
    WritePackageToFile(packageDataFilePtr, appSystem.packageList);
    fclose(userDataFilePtr);
    fclose(packageDataFilePtr);
    freeUsers(&appSystem.userList);
    freePackages(&appSystem.packageList);
    freeInventory(appSystem.inventory);
}

// 主交互界面
void mainMenu() {
    while (appSystem.isRunning) {
        printf("\n=== 菜鸟驿站主菜单 ===\n");
        printf("1. 用户登录\n");
        printf("2. 用户注册\n");
        printf("3. 退出系统\n");
        printf("请选择操作: ");

        int choice;
        scanf("%d", &choice);
        while(getchar() != '\n'); // 清空输入缓冲区

        switch (choice) {
            case 1: userLoginFlow(); break;
            case 2: userRegisterFlow(&appSystem.userList); break;
            case 3: appSystem.isRunning = 0; break;
            default: printf("无效输入，请重新选择！\n");
        }
    }
}

// 用户登录流程
void userLoginFlow() {
    char account[UserNameLen], password[PinLen];
    printf("请输入用户名: \n");
    fgets(account, UserNameLen, stdin);
    printf("请输入密码: \n");
    fgets(password, PinLen, stdin);

    UserData* user = authenticateUser(appSystem.userList, account, password);
    if (user) {
        if (user->permission) {
            adminDashboard(user);
        } else {
            userDashboard(user);
        }
    } else {
        printf("账号或密码错误！");
    }
    return;
}

// 用户功能面板
void userDashboard(UserData* user) {
    while (appSystem.isRunning) {
        printf("\n=== 用户面板 [%s] ===\n", user->userName);
        searchParcelInterface(user, appSystem.packageList);
        printf("操作：\n");
        printf("1. 寄送包裹\n");
        printf("2. 取件\n");
        printf("3. 查看历史包裹\n");
        printf("4. 修改个人信息\n");   
        printf("5. 注销账号\n");
        printf("6. 返回主菜单\n");
        printf("7. 退出系统");
        printf("请选择操作: ");

        int choice;
        scanf("%d", &choice);
        while(getchar() != '\n');
        system("cls");
        switch (choice) {
            case 1: {
                PackageData* p = createParcel(user);
                addParcelToInventory(appSystem.inventory, p);
                break;
            }
            case 2: getParcelFromInventory(user, appSystem.packageList); break;      //查看有无待取包裹
            case 3: displayUserHistory(user, appSystem.packageList); break;
            case 4: modifyUserProfile(user); break;
            case 5: deleteUser(&appSystem.userList, user->userName); break;
            case 6: return;
            case 7: appSystem.isRunning = 0; break;
            default: {
                printf("无效操作！请重新输入选项：\n");
                scanf("%d", &choice);
                while(getchar()!='\n');
            }
        }
        system("cls");
    }
}

// 管理员面板
void adminDashboard(UserData* admin) {
    while (appSystem.isRunning) {
        printf("当前管理员：");
        fputs(admin->name, stdout);
        printf("\n");
        printf("\n=== 管理控制台 ===\n");
        printf("1. 查看库存状态\n");           //按顺序打印各货架当前剩余空间                                  
        printf("2. 包裹寄出登记\n");
        printf("3. 返回主菜单\n");
        printf("4. 退出系统");
        printf("请选择操作: ");

        int choice;
        scanf("%d", &choice);
        while(getchar() != '\n');
        system("cls");
        switch (choice) {
            case 1: displayInventoryStatus(appSystem.inventory); break;
            case 2: sentParcelRecording(&appSystem.packageList); break;
            case 3: return;
            case 4: appSystem.isRunning = 0; break;
            default: {
                printf("无效操作！请重新输入选项：\n");
                scanf("%d", &choice);
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
    printf("系统已安全退出\n");
    return 0;
}