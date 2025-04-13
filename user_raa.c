#include"user_raa.h"

void userRegisterFlow(UserData** userList) {
    // 1. Ϊ���û���̬�����ڴ�
    UserData* newUser = (UserData*)malloc(sizeof(UserData));
    if (newUser == NULL) {
        perror("�ڴ����ʧ��");
        exit(EXIT_FAILURE); // ���ش���ֱ���˳�
    }
    memset(newUser, 0, sizeof(UserData)); // ��ʼ���ڴ�Ϊ0
    
    newUser->numOfDiscount=6; //���û�6���Ż� 
    // 2. Ȩ��ѡ���߼�
    int choosePermission;
    printf("\n����������ѡ���˺����ͣ�1.�û� 2.����Ա\n");
    scanf("%d", &choosePermission);
    if (choosePermission != 1 && choosePermission!=2){ // ����У��
        printf("�����ʽ����\n");
        free(newUser);
        return;
    }
    getchar(); // ������뻺�����Ļ��з�

    // Ȩ��У��ѭ��
    while (1) {
        if (choosePermission == 1) {
            newUser->permission = UserPermission;
            break;
        } else if (choosePermission == 2) {
            newUser->permission = AdminPermission;
            newUser->userType = others;
            break;
        } else {
            printf("������1��2��");
            scanf("%d", &choosePermission);
            if (choosePermission != 1 && choosePermission!=2) {
                printf("�����ʽ����\n");
                free(newUser);
                return;
            }
            getchar();
        }
    }

    // 3. �����û�������Ϣ�������з�����
    printf("���������������%d�ַ�����\n", NameLen-1);
    fgets(newUser->name, NameLen, stdin);
    newUser->name[strcspn(newUser->name, "\n")] = '\0'; // ȥ�����з�
    
    printf("�������˻��������%d�ַ�����\n", UserNameLen-1);
    fgets(newUser->userName, UserNameLen, stdin);
    newUser->userName[strcspn(newUser->userName, "\n")] = '\0';
    //�ظ�ע����
    UserData* judger = *userList;
    while(judger!=NULL){
        if(strcmp(newUser->name,judger->name)==0 || strcmp(newUser->userName,judger->userName)==0){
            printf("���˺���Ϣ�Ѵ��ڣ�\n");
            free(newUser);
            return;
        }
        judger=judger->nextUserData;
    }

    printf("���������루���%d�ַ�����\n", PinLen-1);
    fgets(newUser->pin, PinLen, stdin);
    newUser->pin[strcspn(newUser->pin, "\n")] = '\0';

    // 4. �û�����ѡ��ǿ�ƹ���ԱΪУ����Ա��
    if(newUser->permission==UserPermission){
        int chooseUserType;
        printf("ѡ���û����ͣ�1.ѧ�� 2.��ʦ 3.У����Ա\n");
        scanf("%d", &chooseUserType);
        if (chooseUserType != 1 && chooseUserType != 2 && chooseUserType != 3) {
            printf("�����ʽ����\n");
            free(newUser);
            return;
        }
        getchar();
        int typeJudger=0;
        // �û�����У��ѭ��
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
                    printf("������1-3��");
                    if (scanf("%d", &chooseUserType) != 1) {
                        printf("�����ʽ����\n");
                        free(newUser);
                        return;
                    }
                    getchar();
                    continue; // ����ѭ��У��
            }
        }
    }

    // 5. �����û���ӵ�����β��
    if (*userList == NULL) {
        // ����Ϊ�գ�ֱ������Ϊͷ�ڵ�
        *userList = newUser;
    } else {
        // ����������β��
        UserData* current = *userList;
        while (current->nextUserData != NULL) {
            current = current->nextUserData;
        }
        current->nextUserData = newUser;
    }
    newUser->nextUserData = NULL; // ��ȷ���β�ڵ�

    printf("ע��ɹ���\n");
}

//������������Ƿ��������û�ƥ��
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