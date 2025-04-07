#include"user_raa.h"

void userRegisterFlow(UserData** userList) {
    // 1. 为新用户动态分配内存
    UserData* newUser = (UserData*)malloc(sizeof(UserData));
    if (newUser == NULL) {
        perror("内存分配失败");
        exit(EXIT_FAILURE); // 严重错误直接退出
    }
    memset(newUser, 0, sizeof(UserData)); // 初始化内存为0
    
    newUser->numOfDiscount=6; //新用户6次优惠 
    // 2. 权限选择逻辑
    int choosePermission;
    printf("\n输入数字以选择账号类型：1.用户 2.管理员\n");
    scanf("%d", &choosePermission);
    if (choosePermission != 1 && choosePermission!=2){ // 输入校验
        printf("输入格式错误！\n");
        free(newUser);
        return;
    }
    getchar(); // 清除输入缓冲区的换行符

    // 权限校验循环
    while (1) {
        if (choosePermission == 1) {
            newUser->permission = UserPermission;
            break;
        } else if (choosePermission == 2) {
            newUser->permission = AdminPermission;
            newUser->userType = others;
            break;
        } else {
            printf("请输入1或2：");
            scanf("%d", &choosePermission);
            if (choosePermission != 1 && choosePermission!=2) {
                printf("输入格式错误！\n");
                free(newUser);
                return;
            }
            getchar();
        }
    }

    // 3. 输入用户基本信息（含换行符处理）
    printf("请输入姓名（最多%d字符）：\n", NameLen-1);
    fgets(newUser->name, NameLen, stdin);
    newUser->name[strcspn(newUser->name, "\n")] = '\0'; // 去除换行符
    
    printf("请输入账户名（最多%d字符）：\n", UserNameLen-1);
    fgets(newUser->userName, UserNameLen, stdin);
    newUser->userName[strcspn(newUser->userName, "\n")] = '\0';
    //重复注册检测
    UserData* judger = *userList;
    while(judger!=NULL){
        if(strcmp(newUser->name,judger->name)==0 || strcmp(newUser->userName,judger->userName)==0){
            printf("该账号信息已存在！\n");
            free(newUser);
            return;
        }
        judger=judger->nextUserData;
    }

    printf("请输入密码（最多%d字符）：\n", PinLen-1);
    fgets(newUser->pin, PinLen, stdin);
    newUser->pin[strcspn(newUser->pin, "\n")] = '\0';

    // 4. 用户类型选择（强制管理员为校外人员）
    if(newUser->permission==UserPermission){
        int chooseUserType;
        printf("选择用户类型：1.学生 2.教师 3.校外人员\n");
        scanf("%d", &chooseUserType);
        if (chooseUserType != 1 && chooseUserType != 2 && chooseUserType != 3) {
            printf("输入格式错误！\n");
            free(newUser);
            return;
        }
        getchar();
        int typeJudger=0;
        // 用户类型校验循环
        while (!typeJudger) {
            switch (chooseUserType) {
                case 1:
                    newUser->userType = student;
                    typeJudger=1;
                    break;
                case 2:
                    newUser->userType = teacher;
                    typeJudger=1;
                    break;
                case 3:
                    newUser->userType = others;
                    typeJudger=1;
                    break;
                default:
                    printf("请输入1-3：");
                    if (scanf("%d", &chooseUserType) != 1) {
                        printf("输入格式错误！\n");
                        free(newUser);
                        return;
                    }
                    getchar();
                    continue; // 重新循环校验
            }
        }
    }

    // 5. 将新用户添加到链表尾部
    if (*userList == NULL) {
        // 链表为空，直接设置为头节点
        *userList = newUser;
    } else {
        // 遍历到链表尾部
        UserData* current = *userList;
        while (current->nextUserData != NULL) {
            current = current->nextUserData;
        }
        current->nextUserData = newUser;
    }
    newUser->nextUserData = NULL; // 明确标记尾节点

    printf("注册成功！\n");
}

//检查输入帐密是否与已有用户匹配
UserData* authenticateUser(UserData* user, const char* account, const char* password){
    UserData* current = user;
    while(current!=NULL){
        if(strcmp(account,current->userName)==0 && strcmp(password,current->pin)==0){
            return current;
        }
        current=current->nextUserData;
    }
    return NULL;
}