#include"admin_function.h"

void displayInventoryStatus(InventoryManagement* inventory){
  ShelfNode* currentShelf=inventory->shelves;
  int cnt=0;
  while(currentShelf!=NULL){
    printf("��%d�㹲�а���%d����ʣ��ռ䣺%.2fm\n", currentShelf->level, currentShelf->packageCnt, currentShelf->remainSpace);
    cnt+=currentShelf->packageCnt;
    currentShelf=currentShelf->nextShelfNode;
  }
  printf("�ֿ��ڹ���%d������\n", cnt);
  currentShelf=NULL;
  printf("�����س��Խ�����һ������\n");
  while(getchar()=='\n')return;
}

void sentParcelRecording(PackageData* packageList, InventoryManagement* inventory){
  char sender[NameLen];
  if(StrInputValidation("����",NameLen,0,sender))
    return;
  PackageData* cur= packageList;
  printf("���û������д��İ������˵��ţ�\n");
  while(cur!=NULL){
    if(cur->packageStatus==pendingSend && strcmp(cur->name, sender) == 0){
      printf("%s\n", cur->pickUpCode);
      int choice=0;
      printf("ѡ��1�Գ���ð�����ѡ��0������������Ϣ\n");
      scanf("%d", &choice);
      getchar();
      while(1){
        if(choice==1){
          cur->packageStatus=sent;
          replaceParcelFromInventory(cur, inventory);
          printf("�ð�������ɹ���\n");
          break;
        }else if(choice==0){
          break;
        }else{
          printf("��������ȷ��ѡ�\n");
          scanf("%d", &choice);
          getchar();
        }
      }
    }
    cur=cur->nextPackageData;
  }
  printf("��չʾ���û�ȫ�����İ����������س��Է���\n");
  while(getchar()!='\n');
}
//�ֶ�����µĴ�ȡ����
void addNewParcelToList(PackageData** packageList, InventoryManagement* inventory){
  PackageData* newPackage = (PackageData*)malloc(sizeof(PackageData));
  if (!newPackage) {
    perror("�ڴ����ʧ��");
    return;
  }
  char defaultPickupCode[PickupCodeLen]="0-00-000";   //���ʱ��ȡ���루���ţ�λ�÷���ռλ��
  char defaultAddress[AddressLen]="unknown";
  
  newPackage->nextPackageData=NULL;
  newPackage->packageType=toPickup;
  newPackage->packageStatus=pendingPickup;
  newPackage->fee=0;
  newPackage->weight=0;

  strncpy(newPackage->pickUpCode, defaultPickupCode, PickupCodeLen - 1);
  newPackage->pickUpCode[PickupCodeLen - 1] = '\0';
  strncpy(newPackage->address, defaultAddress, AddressLen-1);
  newPackage->address[AddressLen-1] = '\0';

  //�ռ�������
  if(StrInputValidation("�ռ�������",NameLen,0,newPackage->name)){
    free(newPackage);
    return;
}

  //��װ����
  int choice=0;
  printf("����������İ�װ���ͣ�\n1.�ʼ� 2.С���� 3.�а��� 4.����� 5.�ش����\n");
  scanf("%d", &choice);
  while(getchar() != '\n');
  int choiceIsPass=0;
  while(!choiceIsPass){
    switch(choice){
      case 1:{
        newPackage->packageSize=mail;
        choiceIsPass=1;
        break;
      }
      case 2:{
        newPackage->packageSize=small;
        choiceIsPass=1;
        break;
      }
      case 3:{
        newPackage->packageSize=medium;
        choiceIsPass=1;
        break;
      }
      case 4:{
        newPackage->packageSize=large;
        choiceIsPass=1;
        break;
      }
      case 5:{
        newPackage->packageSize=extraLarge;
        choiceIsPass=1;
        break;
      }
      default:{
        printf("��������ȷ��������ѡ��������ͣ����������룺\n");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  newPackage->volume=PackageSizeToVolume(newPackage->packageSize);
  //���°������뵽�����б�β��
  if (*packageList == NULL) {
    // ����Ϊ�գ�ֱ������Ϊͷ�ڵ�
    *packageList = newPackage;
  } else {
    // ����������β��
    PackageData* current = *packageList;
    while (current!= NULL) {
        current = current->nextPackageData;
    }
    current = newPackage;
  }
  newPackage->nextPackageData = NULL; // ��ȷ���β�ڵ�

  addParcelToInventory(inventory, *packageList);
  printf("���ɹ��������س��Է���\n");
  while(getchar()!='\n');
}

//չʾ�����б�
void displayPackageList(PackageData* packageList){
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
          if(tmpPtr->packageStatus==pendingPickup){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("��ǩ�գ�\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->packageStatus==pickedUp){
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
          if(tmpPtr->packageStatus==pendingSend){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("�Ѽĳ���\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->packageStatus==sent){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("��ȡ����\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->packageStatus==wrong4){
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