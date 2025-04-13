#include"user_function.h"

PackageData* createParcel(PackageData** packageList, UserData* user){
  PackageData* newPackage=(PackageData*)malloc(sizeof(PackageData));
  if (!newPackage) {
    perror("�ڴ����ʧ��");
    return NULL;
  }
  
  newPackage->packageType=toSend;
  newPackage->packageStatus=pendingSend;

  int choice=0;
  float costOfMoney=0;
  float weightOfPackage=0;
  char nameOfPackage[NameLen];
  
  //�ռ�������
  printf("�������ռ���������\n");
  fgets(nameOfPackage, NameLen, stdin);
  while(getchar()!='\n');
  strcpy(newPackage->name, nameOfPackage);

  //���ݰ���������������˷�
  printf("���������������kg����\n");
  scanf("%f", &weightOfPackage);
  while(getchar()!='\n');
  while(weightOfPackage<=0){
    printf("�����������Ϸ������������룺\n");    //��ֹ����Ϊ������0
    scanf("%f", &weightOfPackage);
    while(getchar()!='\n');
  }
  
  if(weightOfPackage<=3){
    costOfMoney+=10;
  }else{
    costOfMoney+=(10+(weightOfPackage-3)*1);
  }
  
  //��װ����
  printf("��������Ҫ�İ�װ�������ͣ�\n1.�ʼ� 2.С���� 3.�а��� 4.����� 5.�ش����");
  scanf("%d", &choice);
  while(getchar() != '\n');
  int choiceIsPass=0;
  while(!choiceIsPass){
    switch(choice){
      case 1:{
        newPackage->packageSize=mail;
        costOfMoney+=1;
        choiceIsPass=1;
        break;
      }
      case 2:{
        newPackage->packageSize=small;
        costOfMoney+=2;
        choiceIsPass=1;
        break;
      }
      case 3:{
        newPackage->packageSize=medium;
        costOfMoney+=5;
        choiceIsPass=1;
        break;
      }
      case 4:{
        newPackage->packageSize=large;
        costOfMoney+=10;
        choiceIsPass=1;
        break;
      }
      case 5:{
        newPackage->packageSize=extraLarge;
        costOfMoney+=15;
        choiceIsPass=1;
        break;
      }
      default:{
        printf("��������ȷ��������ѡ������������ͣ����������룺");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  
  //����ȡ����¥����
  int serviceChoice;
  printf("�Ƿ���Ҫȡ����¥����������������ѡ��\n1.�� 2.��");
  scanf("%d", &serviceChoice);
  choiceIsPass=0;
  while(!choiceIsPass){
    switch (serviceChoice){
      case 1:{
        if(user->userType==student){
          costOfMoney+=2;
        }else if(user->userType==teacher){
          costOfMoney+=1;
        }else if(user->userType==others){
          costOfMoney+=5;
        }
        //ԤԼȡ��ʱ��
        time_t now;
        time(&now); // ��ȡ��ǰʱ���

        // ��ȡ�û����루��ʽ����/�� ʱ:�֣�
        int month, day, hour, minute;
        printf("������һ���������ʱ�䣬���ǽ�����ʱ����ȡ������ʽ MM/DD hh:mm����");
        scanf("%d/%d %d:%d", &month, &day, &hour, &minute);

        // ��ȡ��ǰʱ��� tm �ṹ��
        struct tm *current = localtime(&now);
        struct tm target_tm = *current; // ���Ƶ�ǰʱ��

        // ����������¡��ա�ʱ���֣�ע���·ݴ� 0 ��ʼ��
        target_tm.tm_mon = month - 1;
        target_tm.tm_mday = day;
        target_tm.tm_hour = hour;
        target_tm.tm_min = minute;
        target_tm.tm_sec = 0; // ����Ϊ 0

        // ת��Ϊ time_t ʱ���
        time_t target_time = mktime(&target_tm);

        // ������꣺������ʱ���ѹ���������һ���ͬһʱ��
        if (target_time < now) {
          target_tm.tm_year += 1; // ��� +1
          target_time = mktime(&target_tm);
        }
        // ���ת������·ݺ������Ƿ��������һ��
        if (target_tm.tm_mon + 1 != month || target_tm.tm_mday != day) {
          printf("�����������Ч��\n");
        }

        // �Ƚ�ʱ��
        if (target_time == -1) {
          printf("������Ч�����ڣ�\n");
        } else if (target_time < now) {
          printf("�����ʱ���ѹ���\n");
        } else {
          printf("ԤԼ�ɹ���\n");
        }
        choiceIsPass=1;
        break;
      }
      case 2:{
        choiceIsPass=1;
        break;
      }
      default:{               //������Ƿ���ѭ������÷���Ҫ�������
        printf("������1��2��ѡ��");
        scanf("%d", &serviceChoice);
        while(getchar()!='\n');
      }
    }
  }
  
  
  //���û��������Ż�
  if(user->numOfDiscount){
    if(user->userType==student){
      costOfMoney*=0.7;
    }else if(user->userType==teacher){
      costOfMoney*=0.5;
    }
    user->numOfDiscount--;
  }
  //�������ÿ��20��2��
  costOfMoney-=(int)(costOfMoney/20)*2;

  newPackage->fee=costOfMoney;
  printf("���İ��������˷�Ϊ��%.2fԪ", costOfMoney);
  
  //���°������뵽�����б�β��
  if (*packageList == NULL) {
    // ����Ϊ�գ�ֱ������Ϊͷ�ڵ�
    *packageList = newPackage;
  } else {
    // ����������β��
    PackageData* current = *packageList;
    while (current->nextPackageData != NULL) {
        current = current->nextPackageData;
    }
    current->nextPackageData = newPackage;
  }
  newPackage->nextPackageData = NULL; // ��ȷ���β�ڵ�
  
  return newPackage;
}

void searchParcelInterface(UserData* user, PackageData* packageList){
  PackageData* tmpPtr=packageList;
  int cnt=0;
  printf("��Щ�������������ģ�\n");
  while(tmpPtr!=NULL){
    if(user->name==tmpPtr->name && tmpPtr->packageStatus==pendingPickup){
      printf("%s\n", tmpPtr->pickUpCode);
      cnt++;
    }
  }
  if(!cnt){
    printf("\n���޴�ȡ����\n");
  }
  tmpPtr=NULL;
  free(tmpPtr);
  return;
}

void getParcelFromInventory(UserData* user, PackageData* packageList, InventoryManagement* inventory){
  char targetPickUpCode[PickupCodeLen];
  printf("������ȡ���룺\n");
  fgets(targetPickUpCode, PickupCodeLen, stdin);
  PackageData* tmpPtr=packageList;
  int targetIsFound=0;
  while(tmpPtr!=NULL){
    if(strcmp(tmpPtr->pickUpCode, targetPickUpCode) == 0){
      targetIsFound=1;
      break;
    }
    tmpPtr=tmpPtr->nextPackageData;
  }
  if(!targetIsFound){
    printf("�����ȡ���룡");
    while(getchar()!='\n');
    return;
  }else{
    printf("�ҵ�����������������������ȡ�߰�����\n");
    char targetPassword[PinLen];
    fgets(targetPassword, PinLen, stdin);
    int passwordIsCorrect=0;
    while(!passwordIsCorrect){
      if(targetPassword==user->pin){
        passwordIsCorrect=1;
        tmpPtr->packageStatus=pickedUp;
        replaceParcelFromInventory(tmpPtr, inventory);
        printf("������ȡ����\n");
      }else{
        printf("��������룡���������룺\n");
        fgets(targetPassword, PinLen, stdin);
      }
    }
  }
  tmpPtr=NULL;
  free(tmpPtr);
  return;
}
//��ѯĳλ�û�����ʷȡ����ĳ�������������ʾ
void displayUserHistory(UserData* user, PackageData* packageList){
  int choice;
  printf("������������ѡ����Ҫ���ҵļ�¼���ͣ�1.�ռ� 2.�ļ�\n");
  scanf("%d", &choice);
  while(getchar()!='\n');
  PackageData* tmpPtr=packageList;
  int choiceIsPass=0;
  while(!choiceIsPass){
    switch(choice){
      case 1:{    //��ѯ�����յ��İ���
        choiceIsPass=1;
        printf("��ȡ����\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pendingPickup){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("��ǩ�գ�\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pickedUp){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        break;  
      }
      case 2:{
        choiceIsPass=1;
        printf("δ�ĳ���\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pendingSend){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("�Ѽĳ���\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==sent){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("��ȡ����\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==wrong4){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        break;
      }
      default:{            //������Ƿ���ѭ������÷���Ҫ�������
        printf("������1��2��ѡ��");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  
  tmpPtr=NULL;
  free(tmpPtr);
  while(getchar()=='\n')return;
}

void modifyUserProfile(UserData* user){
  int choice=0;
  int choiceIsPass=0;
  printf("����������������ѡ����Ҫ�޸ĵ���Ϣ��1.�û��� 2������\n");
  scanf("%d", &choice);
  while(!choiceIsPass){
    switch (choice){
      case 1:{
        choiceIsPass=1;
        printf("�������������û�����21���ַ����ڣ���\n");
        fgets(user->userName, UserNameLen, stdin);
        user->userName[strcspn(user->userName,"\n")]='\0';
        printf("�����û����޸ĳɹ���");
        break;
      }
      case 2:{
        choiceIsPass=1;
        char newPassword[PinLen], newPasswordConfirmer[PinLen];
        printf("���������������루21���ַ����ڣ���\n");
        fgets(newPassword, PinLen, stdin);
        printf("���ٴ�����������ȷ�ϣ�\n");
        fgets(newPasswordConfirmer, PinLen, stdin);
        int confirmer=0;
        while(!confirmer){
          if(strcmp(newPassword, newPasswordConfirmer)==0){
            confirmer=1;
            newPassword[strcspn(newPassword, "\n")]='\0';
            strcpy(user->pin, newPassword);
            printf("���������޸ĳɹ���");
            break;
          }else{
            printf("�������벻�������������룺\n");
            fgets(newPasswordConfirmer, PinLen, stdin);
          }
        }
        break;
      }
      default:{               //������Ƿ���ѭ������÷���Ҫ�������
        printf("������1��2��ѡ��\n");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  } 
}

//ע���˺�
void deleteUser(UserData** userList, const char* userName) {
  int choice=1;
  int choiceIsPass=0;
  printf("���Ƿ�Ҫע���˺ţ�����1�Լ���������0�Է����û��˵���\n");   //����ڻ���
  scanf("%d", &choice);  
  while(!choiceIsPass){
    switch (choice){
      case 1:{
        choiceIsPass=1;
        break;
      }
      case 0:{
        return;
      }
      default:{               //������Ƿ���ѭ������÷���Ҫ�������
        printf("������1��0��ѡ��\n");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  } 

  UserData* current = *userList;
    UserData* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->userName, userName) == 0) {
            if (prev == NULL) {
                *userList = current->nextUserData; // �޸�ͷָ��
            } else {
                prev->nextUserData = current->nextUserData;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->nextUserData;
    }
}