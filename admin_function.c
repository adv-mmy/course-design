#include"admin_function.h"

void displayInventoryStatus(InventoryManagement* inventory){
  ShelfNode* currentShelf=inventory->shelves;
  int cnt=0;
  while(currentShelf!=NULL){
    printf("第%d层共有包裹%d件，剩余空间：%fm\n", currentShelf->level, currentShelf->packageCnt, currentShelf->remainSpace);
    cnt+=currentShelf->packageCnt;
  }
  printf("仓库内共有%d件包裹\n", cnt);
  currentShelf=NULL;
  free(currentShelf);
  return;
}

void sentParcelRecording(PackageData** packageList){
  char sender[NameLen];
  printf("请输入寄件人姓名：\n");
  fgets(sender, NameLen, stdin);
  
  PackageData* cur= *packageList;
  while(cur!=NULL){
    if(cur->packageStatus==pendingSend){
      printf("寄往%s的包裹\n", &cur->address);
      int choice=0;
      printf("选择1以出库该包裹，选择0以跳过该条信息\n");
      scanf("%d", &choice);
      getchar();
      while(1){
        if(choice==1){
          cur->packageStatus=sent;
          break;
        }else if(choice==0){
          break;
        }else{
          printf("请输入正确的选项！\n");
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
  newPackage->nextPackageData=NULL;
  newPackage->packageStatus=pendingPickup;
  newPackage->fee=0;

  //收件人姓名
  printf("请输入收件人姓名：\n");
  fgets(newPackage->name, NameLen, stdin);

  //包装类型
  int choice=0;
  printf("请输入包裹的包装类型：\n1.邮件 2.小包裹 3.中包裹 4.大包裹 5.特大包裹");
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
        printf("请输入正确的数字以选择包裹类型！请重新输入：");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  addParcelToInventory(inventory, packageList);
}