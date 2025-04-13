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

void sentParcelRecording(PackageData** packageList){
  char sender[NameLen];
  printf("������ļ���������\n");
  fgets(sender, NameLen, stdin);
  
  PackageData* cur= *packageList;
  while(cur!=NULL){
    if(cur->packageStatus==pendingSend){
      printf("����%s�İ���\n", cur->address);
      int choice=0;
      printf("ѡ��1�Գ���ð�����ѡ��0������������Ϣ\n");
      scanf("%d", &choice);
      getchar();
      while(1){
        if(choice==1){
          cur->packageStatus=sent;
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
}

void addNewParcelToList(PackageData** packageList, InventoryManagement* inventory){
  PackageData* newPackage = (PackageData*)malloc(sizeof(PackageData));
  if (!newPackage) {
    perror("�ڴ����ʧ��");
    return;
  }
  char defaultPickupCode[PickupCodeLen]="A-00-000";   //���ʱ��ȡ���루���ţ�λ�÷���ռλ��
  newPackage->nextPackageData=NULL;
  newPackage->packageStatus=pendingPickup;
  newPackage->fee=0;
  strncpy(newPackage->pickUpCode, defaultPickupCode, PickupCodeLen - 1);
  newPackage->pickUpCode[PickupCodeLen - 1] = '\0';

  //�ռ�������
  printf("�������ռ���������\n");
  fgets(newPackage->name, NameLen, stdin);

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

  addParcelToInventory(inventory, *packageList);
}