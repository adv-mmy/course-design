#include"admin_function.h"

void displayInventoryStatus(InventoryManagement* inventory){
  ShelfNode* currentShelf=inventory->shelves;
  int cnt=0;
  while(currentShelf!=NULL){
    printf("第%d层共有包裹%d件，剩余空间：%.2fm\n", currentShelf->level, currentShelf->packageCnt, currentShelf->remainSpace);
    cnt+=currentShelf->packageCnt;
    currentShelf=currentShelf->nextShelfNode;
  }
  printf("仓库内共有%d件包裹\n", cnt);
  currentShelf=NULL;
  printf("单击回车以进行下一步操作\n");
  while(getchar()=='\n')return;
}

void sentParcelRecording(PackageData* packageList, InventoryManagement* inventory){
  char sender[NameLen];
  if(StrInputValidation("姓名",NameLen,0,sender))
    return;
  PackageData* cur= packageList;
  printf("该用户的所有待寄包裹的运单号：\n");
  while(cur!=NULL){
    if(cur->packageStatus==pendingSend && strcmp(cur->name, sender) == 0){
      printf("%s\n", cur->pickUpCode);
      int choice=0;
      printf("选择1以出库该包裹，选择0以跳过该条信息\n");
      scanf("%d", &choice);
      getchar();
      while(1){
        if(choice==1){
          cur->packageStatus=sent;
          replaceParcelFromInventory(cur, inventory);
          printf("该包裹出库成功！\n");
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
  printf("已展示该用户全部待寄包裹，单击回车以返回\n");
  while(getchar()!='\n');
}
//手动添加新的待取包裹
void addNewParcelToList(PackageData** packageList, InventoryManagement* inventory){
  PackageData* newPackage = (PackageData*)malloc(sizeof(PackageData));
  if (!newPackage) {
    perror("内存分配失败");
    return;
  }
  char defaultPickupCode[PickupCodeLen]="0-00-000";   //入库时在取件码（单号）位置分配占位符
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

  //收件人姓名
  if(StrInputValidation("收件人姓名",NameLen,0,newPackage->name)){
    free(newPackage);
    return;
}

  //包装类型
  int choice=0;
  printf("请输入包裹的包装类型：\n1.邮件 2.小包裹 3.中包裹 4.大包裹 5.特大包裹\n");
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
        printf("请输入正确的数字以选择包裹类型！请重新输入：\n");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  newPackage->volume=PackageSizeToVolume(newPackage->packageSize);
  //将新包裹加入到包裹列表尾部
  if (*packageList == NULL) {
    // 链表为空，直接设置为头节点
    *packageList = newPackage;
  } else {
    // 遍历到链表尾部
    PackageData* current = *packageList;
    while (current!= NULL) {
        current = current->nextPackageData;
    }
    current = newPackage;
  }
  newPackage->nextPackageData = NULL; // 明确标记尾节点

  addParcelToInventory(inventory, *packageList);
  printf("入库成功！单击回车以返回\n");
  while(getchar()!='\n');
}

//展示包裹列表
void displayPackageList(PackageData* packageList){
  int choice;
  printf("请输入数字以选择您要查找的记录类型：\n1.收件 2.寄件\n");
  scanf("%d", &choice);
  while(getchar()!='\n');
  PackageData* tmpPtr=packageList;
  int choiceIsPass=0;
  while(!choiceIsPass){
    switch(choice){
      case 1:{    //查询所有收到的包裹
        choiceIsPass=1;
        printf("待取件：\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->packageStatus==pendingPickup){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("已签收：\n");
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
        printf("未寄出：\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->packageStatus==pendingSend){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("已寄出：\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->packageStatus==sent){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("已取消：\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->packageStatus==wrong4){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        break;
      }
      default:{            //若输入非法则循环到获得符合要求的输入
        printf("请输入1或2以选择：");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  
  tmpPtr=NULL;
  free(tmpPtr);
  printf("已展示所有历史记录，单击回车以返回\n");
  while(getchar()=='\n')return;
}