#include"user_function.h"

PackageData* createParcel(UserData* user){
  PackageData* newPackage=(PackageData*)malloc(sizeof(PackageData));
  newPackage->packageType=toSend;
  newPackage->packageStatus=pendingSend;

  int choice;
  float costOfMoney=0;
  float weightOfPackage;
  char nameOfPackage[NameLen];
  
  //收件人姓名
  printf("请输入收件人姓名：\n");
  fgets(nameOfPackage, NameLen, stdin);
  while(getchar()!='\n');
  strcpy(newPackage->name, nameOfPackage);

  //根据包裹重量计算基础运费
  printf("请输入包裹重量（kg）：\n");
  scanf("%f", &weightOfPackage);
  while(getchar()!='\n');
  while(weightOfPackage<=0){
    printf("包裹重量不合法！请重新输入：\n");    //防止输入为负数或0
    scanf("%f", &weightOfPackage);
    while(getchar()!='\n');
  }
  
  if(weightOfPackage<=3){
    costOfMoney+=10;
  }else{
    costOfMoney+=(10+(weightOfPackage-3)*1);
  }
  
  //包装类型
  printf("请输入需要的包装物料类型：\n1.邮件 2.小包裹 3.中包裹 4.大包裹 5.特大包裹");
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
        printf("请输入正确的数字以选择包裹物料类型！请重新输入：");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  
  //新增取件到楼服务
  int serviceChoice;
  printf("是否需要取件到楼服务？请输入数字以选择：\n1.是 2.否");
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
      choiceIsPass=1;
      break;
    }
    case 2:{
      choiceIsPass=1;
      break;
    }
    default:{               //若输入非法则循环到获得符合要求的输入
      printf("请输入1或2以选择：");
      scanf("%d", &serviceChoice);
      while(getchar()!='\n');
    }
  }
  }
  
  //新用户有六次优惠
  if(user->numOfDiscount){
    if(user->userType==student){
      costOfMoney*=0.7;
    }else if(user->userType==teacher){
      costOfMoney*=0.5;
    }
    user->numOfDiscount--;
  }
  //活动满减（每满20减2）
  costOfMoney-=(int)(costOfMoney/20)*2;

  newPackage->fee=costOfMoney;
  printf("您的包裹所需运费为：%.2f元", costOfMoney);
  
  return newPackage;
}

void searchParcelInterface(UserData* user, PackageData* packageList){
  PackageData* tmpPtr=packageList;
  printf("这些包裹可能是您的：\n");
  while(tmpPtr->nextPackageData!=NULL){
    if(user->name==tmpPtr->name && tmpPtr->packageStatus==pendingPickup){
      printf("%s\n", tmpPtr->pickUpCode);
    }
  }
  tmpPtr=NULL;
  free(tmpPtr);
  return;
}

void getParcelFromInventory(UserData* user, PackageData* packageList){
  char targetPickUpCode[PickupCodeLen];
  printf("请输入取件码：\n");
  fgets(targetPickUpCode, PickupCodeLen, stdin);
  PackageData* tmpPtr=packageList;
  int targetIsFound=0;
  while(tmpPtr->nextPackageData!=NULL){
    if(tmpPtr->pickUpCode==targetPickUpCode){
      targetIsFound=1;
      break;
    }
    tmpPtr=tmpPtr->nextPackageData;
  }
  if(!targetIsFound){
    printf("错误的取件码！");
    return;
  }else{
    printf("找到包裹！请输入密码以取走包裹：\n");
    char targetPassword[PinLen];
    fgets(targetPassword, PinLen, stdin);
    int passwordIsCorrect=0;
    while(!passwordIsCorrect){
      if(targetPassword==user->pin){
        passwordIsCorrect=1;
        tmpPtr->packageStatus=pickedUp;
        printf("包裹已取出！");
      }else{
        printf("错误的密码！请重新输入：\n");
        fgets(targetPassword, PinLen, stdin);
      }
    }
  }
  tmpPtr=NULL;
  free(tmpPtr);
  return;
}
//查询某位用户的历史取出或寄出包裹并分类显示
void displayUserHistory(UserData* user, PackageData* packageList){
  int choice;
  printf("请输入数字以选择您要查找的记录类型：1.收件 2.寄件\n");
  scanf("%d", &choice);
  while(getchar()!='\n');
  PackageData* tmpPtr=packageList;
  int choiceIsPass=0;
  while(!choiceIsPass){
    switch(choice){
      case 1:{    //查询所有收到的包裹
        choiceIsPass=1;
        printf("待取件：\n");
        while(tmpPtr->nextPackageData!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pendingPickup){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("已签收：\n");
        while(tmpPtr->nextPackageData!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pickedUp){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        break;  
      }
      case 2:{
        choiceIsPass=1;
        printf("未寄出：\n");
        while(tmpPtr->nextPackageData!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pendingSend){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("已寄出：\n");
        while(tmpPtr->nextPackageData!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==sent){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
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
  return;
}

void modifyUserProfile(UserData* user){
  int choice=0;
  int choiceIsPass=0;
  printf("请输入输入数字以选择您要修改的信息：1.用户名 2，密码\n");
  scanf("%d", &choice);
  while(!choiceIsPass){
    switch (choice){
      case 1:{
        choiceIsPass=1;
        printf("请输入您的新用户名（21个字符以内）：\n");
        fgets(user->userName, UserNameLen, stdin);
        printf("您的用户名修改成功！");
        break;
      }
      case 2:{
        choiceIsPass=1;
        char newPassword[PinLen], newPasswordConfirmer[PinLen];
        printf("请输入您的新密码（21个字符以内）：\n");
        fgets(newPassword, PinLen, stdin);
        printf("请再次输入密码以确认：\n");
        fgets(newPasswordConfirmer, PinLen, stdin);
        int confirmer=0;
        while(!confirmer){
          if(strcmp(newPassword, newPasswordConfirmer)==0){
            confirmer=1;
            printf("您的密码修改成功！");
            break;
          }else{
            printf("输入密码不符！请重新输入：\n");
            fgets(newPasswordConfirmer, PinLen, stdin);
          }
        }
        break;
      }
      default:{               //若输入非法则循环到获得符合要求的输入
        printf("请输入1或2以选择：\n");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  } 
}

//注销账号
void deleteUser(UserData** userList, const char* userName) {
    UserData* current = *userList;
    UserData* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->userName, userName) == 0) {
            if (prev == NULL) {
                *userList = current->nextUserData; // 修改头指针
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