#include"user_function.h"

PackageData* createParcel(PackageData** packageList, UserData* user){
  PackageData* newPackage=(PackageData*)malloc(sizeof(PackageData));
  if (!newPackage) {
    perror("内存分配失败");
    return NULL;
  }
  
  newPackage->packageType=toSend;
  newPackage->packageStatus=pendingSend;

  int choice=0;
  float costOfMoney=0;
  float weightOfPackage=0;
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
        //预约取件时间
        time_t now;
        time(&now); // 获取当前时间戳

        // 读取用户输入（格式：月/日 时:分）
        int month, day, hour, minute;
        printf("请输入一个您方便的时间，我们将于那时上门取件（格式 MM/DD hh:mm）：");
        scanf("%d/%d %d:%d", &month, &day, &hour, &minute);

        // 获取当前时间的 tm 结构体
        struct tm *current = localtime(&now);
        struct tm target_tm = *current; // 复制当前时间

        // 设置输入的月、日、时、分（注意月份从 0 开始）
        target_tm.tm_mon = month - 1;
        target_tm.tm_mday = day;
        target_tm.tm_hour = hour;
        target_tm.tm_min = minute;
        target_tm.tm_sec = 0; // 秒设为 0

        // 转换为 time_t 时间戳
        time_t target_time = mktime(&target_tm);

        // 处理跨年：若输入时间已过，则尝试下一年的同一时间
        if (target_time < now) {
          target_tm.tm_year += 1; // 年份 +1
          target_time = mktime(&target_tm);
        }
        // 检查转换后的月份和日期是否与输入的一致
        if (target_tm.tm_mon + 1 != month || target_tm.tm_mday != day) {
          printf("输入的日期无效。\n");
        }

        // 比较时间
        if (target_time == -1) {
          printf("错误：无效的日期！\n");
        } else if (target_time < now) {
          printf("输入的时间已过。\n");
        } else {
          printf("预约成功！\n");
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
  
  //将新包裹加入到包裹列表尾部
  if (*packageList == NULL) {
    // 链表为空，直接设置为头节点
    *packageList = newPackage;
  } else {
    // 遍历到链表尾部
    PackageData* current = *packageList;
    while (current->nextPackageData != NULL) {
        current = current->nextPackageData;
    }
    current->nextPackageData = newPackage;
  }
  newPackage->nextPackageData = NULL; // 明确标记尾节点
  
  return newPackage;
}

void searchParcelInterface(UserData* user, PackageData* packageList){
  PackageData* tmpPtr=packageList;
  int cnt=0;
  printf("这些包裹可能是您的：\n");
  while(tmpPtr!=NULL){
    if(user->name==tmpPtr->name && tmpPtr->packageStatus==pendingPickup){
      printf("%s\n", tmpPtr->pickUpCode);
      cnt++;
    }
  }
  if(!cnt){
    printf("\n暂无待取包裹\n");
  }
  tmpPtr=NULL;
  free(tmpPtr);
  return;
}

void getParcelFromInventory(UserData* user, PackageData* packageList, InventoryManagement* inventory){
  char targetPickUpCode[PickupCodeLen];
  printf("请输入取件码：\n");
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
    printf("错误的取件码！");
    while(getchar()!='\n');
    return;
  }else{
    printf("找到包裹！请输入您的密码以取走包裹：\n");
    char targetPassword[PinLen];
    fgets(targetPassword, PinLen, stdin);
    int passwordIsCorrect=0;
    while(!passwordIsCorrect){
      if(targetPassword==user->pin){
        passwordIsCorrect=1;
        tmpPtr->packageStatus=pickedUp;
        replaceParcelFromInventory(tmpPtr, inventory);
        printf("包裹已取出！\n");
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
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pendingPickup){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("已签收：\n");
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
        printf("未寄出：\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==pendingSend){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("已寄出：\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==sent){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
        }
        tmpPtr=packageList;
        printf("已取消：\n");
        while(tmpPtr!=NULL){
          if(tmpPtr->name== user->name && tmpPtr->packageStatus==wrong4){
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
  while(getchar()=='\n')return;
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
        user->userName[strcspn(user->userName,"\n")]='\0';
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
            newPassword[strcspn(newPassword, "\n")]='\0';
            strcpy(user->pin, newPassword);
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
  int choice=1;
  int choiceIsPass=0;
  printf("您是否要注销账号？输入1以继续，输入0以返回用户菜单：\n");   //给后悔机会
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
      default:{               //若输入非法则循环到获得符合要求的输入
        printf("请输入1或0以选择：\n");
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