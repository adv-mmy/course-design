#include"user_function.h"

void sendParcel(PackageData** packageList, UserData* user, InventoryManagement* inventory){
  PackageData* newPackage=(PackageData*)malloc(sizeof(PackageData));
  if (!newPackage) {
    perror("�ڴ����ʧ��");
    return;
  }
  
  strcpy(newPackage->name, user->name);
  newPackage->packageType=toSend;
  newPackage->packageStatus=pendingSend;
  strcpy(newPackage->pickUpCode, "0-00-000");

  int choice=0;
  float costOfMoney=0;
  float weightOfPackage=0;
  char recieverName[NameLen];
  
  //�ռ�������
  if(StrInputValidation("�ռ�������",NameLen,0,recieverName)){
        free(newPackage);
        return;
    }
  //�ռ���ַ
  if(StrInputValidation("��ַ",UserNameLen,0,newPackage->address)){
        free(newPackage);
        return;
    }

  //���ݰ���������������˷�
  printf("���������������kg����\n");
  scanf("%f", &weightOfPackage);
  while(getchar()!='\n');
  while(weightOfPackage<=0){
    printf("�����������Ϸ������������룺\n");    //��ֹ����Ϊ������0
    scanf("%f", &weightOfPackage);

  }
  newPackage->weight=weightOfPackage;
  
  if(weightOfPackage<=3){
    costOfMoney+=10;
  }else{
    costOfMoney+=(10+(weightOfPackage-3)*1);
  }
  
  //��װ����
  printf("��������Ҫ�İ�װ�������ͣ�\n1.�ʼ���1Ԫ�� 2.С������2Ԫ�� 3.�а�����5Ԫ�� 4.�������10Ԫ�� 5.�ش������15Ԫ��\n");
  scanf("%d", &choice);

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
        printf("��������ȷ��������ѡ������������ͣ����������룺\n");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  newPackage->volume = PackageSizeToVolume(newPackage->packageSize);
  
  //����ȡ����¥����
  int serviceChoice;
  printf("�Ƿ���Ҫȡ����¥����������������ѡ��\n1.�� 2.��\n");
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
        int timeIsLegal=0;
        while(!timeIsLegal){
          // ��ȡ�û����루��ʽ����/�� ʱ:�֣�
          int month, day, hour, minute;
          printf("������һ���������ʱ�䣬���ǽ�����ʱ����ȡ������ʽ MM/DD hh:mm����\n");
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
            while(getchar()!='\n');
            continue;
          }

          // �Ƚ�ʱ��
          if (target_time == -1) {
            printf("������Ч�����ڣ�\n");
            while(getchar()!='\n');
            continue;
          } else if (target_time < now) {
            printf("�����ʱ���ѹ���\n");
            while(getchar()!='\n');
            continue;
          } else {
            printf("ԤԼ�ɹ���\n");
            timeIsLegal=1;
            while(getchar()!='\n');
            continue;
          }
        }
        getchar();
        char targetAddress[AddressLen];
        if(StrInputValidation("��ϣ����ȡ����ַ", AddressLen, 0, targetAddress)){
          return;
        }
        choiceIsPass=1;
        break;
      }
      case 2:{
        choiceIsPass=1;
        break;
      }
      default:{               //������Ƿ���ѭ������÷���Ҫ�������
        printf("������1��2��ѡ��\n");
        scanf("%d", &serviceChoice);
        while(getchar()!='\n');
      }
    }
  }
  
  
  //���û��������Ż�
  if(user->numOfDiscount){
    if(user->userType==student){
      costOfMoney*=0.7f;
    }else if(user->userType==teacher){
      costOfMoney*=0.5f;
    }
    user->numOfDiscount--;
  }
  //�������ÿ��20��2��
  printf("���������Żݻ���У��˷�ÿ��20Ԫ��2Ԫ�����İ����Ż�ǰ�˷�Ϊ��%.2f��", costOfMoney);
  costOfMoney-=(int)(costOfMoney/20)*2;

  newPackage->fee=costOfMoney;
  printf("�Żݺ��˷�Ϊ��%.2fԪ�������س��Է���\n", costOfMoney);
  getchar();
  while(getchar()!='\n');
  
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
  
  addParcelToInventory(inventory, newPackage);                    //��������������

  return;
}

void searchParcelInterface(UserData* user, PackageData* packageList){
  PackageData* tmpPtr=packageList;
  int cnt=0;
  printf("��Щ�������������ģ�\n");
  while(tmpPtr!=NULL){
    if(strcmp(user->name, tmpPtr->name) == 0 && tmpPtr->packageStatus==pendingPickup){
      printf("%s\n", tmpPtr->pickUpCode);
      cnt++;
    }
    tmpPtr=tmpPtr->nextPackageData;
  }
  if(!cnt){
    printf("\n**���޴�ȡ����**\n");
  }
  tmpPtr=NULL;
  free(tmpPtr);
  return;
}

void getParcelFromInventory(UserData* user, PackageData* packageList, InventoryManagement* inventory){
  char targetPickUpCode[PickupCodeLen];
  if(StrInputValidation("ȡ����", PickupCodeLen, 1, targetPickUpCode)){
    return;
  }
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
    printf("�����ȡ���룡�����س��Է���\n");
    while(getchar()!='\n');
    return;
  }else{
    printf("�ҵ�����������������������ȡ�߰���!\n");
    char targetPassword[PinLen];
    if(StrInputValidation("��������", PinLen, 1, targetPassword)){
      return;
    }
    int passwordIsCorrect=0;
    while(!passwordIsCorrect){
      if(strcmp(targetPassword, user->pin) == 0){
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
  //����ȡ����¥����
  int serviceChoice;
  printf("�Ƿ���Ҫ�ͼ���¥����������������ѡ��\n1.�� 2.��\n");
  scanf("%d", &serviceChoice);
  int choiceIsPass=0;
  while(!choiceIsPass){
    switch (serviceChoice){
      case 1:{
        //ԤԼȡ��ʱ��
        time_t now;
        time(&now); // ��ȡ��ǰʱ���
        int timeIsLegal=0;
        while(!timeIsLegal){
          // ��ȡ�û����루��ʽ����/�� ʱ:�֣�
          int month, day, hour, minute;
          printf("������һ���������ʱ�䣬���ǽ�����ʱ�����ͼ�����ʽ MM/DD hh:mm����\n");
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
            while(getchar()!='\n');
            continue;
          }

          // �Ƚ�ʱ��
          if (target_time == -1) {
            printf("������Ч�����ڣ�\n");
            while(getchar()!='\n');
            continue;
          } else if (target_time < now) {
            printf("�����ʱ���ѹ���\n");
            while(getchar()!='\n');
            continue;
          } else {
            printf("ԤԼ�ɹ���\n");
            timeIsLegal=1;
            while(getchar()!='\n');
            continue;
          }
        }
        getchar();
        char targetAddress[AddressLen];
        if(StrInputValidation("��ϣ�����ͼ���ַ", AddressLen, 0, targetAddress)){
          return;
        }
        choiceIsPass=1;
        break;
      }
      case 2:{
        choiceIsPass=1;
        break;
      }
      default:{               //������Ƿ���ѭ������÷���Ҫ�������
        printf("������1��2��ѡ��\n");
        scanf("%d", &serviceChoice);
        while(getchar()!='\n');
      }
    }
  }

  tmpPtr=NULL;
  free(tmpPtr);
  return;
}
//ȡ���ļ�
void cancelSending(UserData* user, PackageData** packageList, InventoryManagement* inventory){
  if (*packageList == NULL) {
    printf("δ���ҵ�������¼\n");
    while(getchar()=='\n')return;
  }

  PackageData* current = *packageList;
  int found = 0;

  char targetPickUpCode[PickupCodeLen];
  StrInputValidation("��Ҫȡ���İ������˵���", PickupCodeLen, 1, targetPickUpCode);

  while (current != NULL) {
    if (strcmp(current->name, user->name) == 0 && strcmp(current->pickUpCode, targetPickUpCode) == 0 && current->packageStatus == pendingSend) {
        found = 1;
        current->packageStatus = canceled;
        replaceParcelFromInventory(current, inventory);
        break;
    }
    current = current->nextPackageData;
  }
  if (!found) {
    printf("δ�ҵ��ð���\n");
    while(getchar()=='\n')return;
  }
  printf("�ɹ�ȡ���ļ����뼰ʱȡ�����İ���\n");
  while(getchar()=='\n')return;
}
//��ѯĳλ�û�����ʷȡ����ĳ�������������ʾ
void displayUserHistory(UserData* user, PackageData* packageList){
  int choice;
  printf("������������ѡ����Ҫ���ҵļ�¼���ͣ�\n1.�ռ� 2.�ļ�\n");
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
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==pendingPickup){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("��ǩ�գ�\n");
        while(tmpPtr!=NULL){
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==pickedUp){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        break;  
      }
      case 2:{
        choiceIsPass=1;
        printf("δ�ĳ���\n");
        while(tmpPtr!=NULL){
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==pendingSend){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("�Ѽĳ���\n");
        while(tmpPtr!=NULL){
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==sent){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("��ȡ����\n");
        while(tmpPtr!=NULL){
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==canceled){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
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
  printf("��չʾ������ʷ��¼�������س��Է���\n");
  while(getchar()=='\n')return;
}

void modifyUserProfile(UserData* user){
  int choice=0;
  int choiceIsPass=0;
  printf("������������ѡ����Ҫ�޸ĵ���Ϣ��\n1.�û��� 2������\n");
  scanf("%d", &choice);
  getchar();
  while(!choiceIsPass){
    switch (choice){
      case 1:{
        choiceIsPass=1;
        if(StrInputValidation("�������û���",UserNameLen,0,user->userName)){
          return;
        }
        printf("�����û����޸ĳɹ���\n");
        break;
      }
      case 2:{
        choiceIsPass=1;
        char newPassword[PinLen], newPasswordConfirmer[PinLen];
        if(StrInputValidation("����������",PinLen,1,newPassword)){
          return;
        }
        if(StrInputValidation("������������ȷ��",PinLen,1,newPasswordConfirmer)){
          return;
        }
        int confirmer=0;
        while(!confirmer){
          if(strcmp(newPassword, newPasswordConfirmer)==0){
            confirmer=1;
            newPassword[strcspn(newPassword, "\n")]='\0';
            strcpy(user->pin, newPassword);
            printf("���������޸ĳɹ���\n");
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
//չʾ��ǰ�û���Ϣ
void displayUserInformation(UserData* user){
  printf("ʵ����Ϣ��%s\n", user->name);
  printf("�˺ţ�%s\n", user->userName);
  printf("�û����ͣ�%s\n", UserTypeToStr(user->userType));
  printf("���룺%s\n", user->pin);
  printf("���û��Ż�ʣ�������%d\n", user->numOfDiscount);
  getchar();
}