#include"user_function.h"

void sendParcel(PackageData** packageList, UserData* user, InventoryManagement* inventory){
  PackageData* newPackage=(PackageData*)malloc(sizeof(PackageData));
  if (!newPackage) {
    perror("内存分配失败");
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
  
  //收件人姓名
  if(StrInputValidation("收件人姓名",NameLen,0,recieverName)){
        free(newPackage);
        return;
    }
  //收件地址
  if(StrInputValidation("地址",UserNameLen,0,newPackage->address)){
        free(newPackage);
        return;
    }

  //根据包裹重量计算基础运费
  printf("请输入包裹重量（kg）：\n");
  scanf("%f", &weightOfPackage);
  while(getchar()!='\n');
  while(weightOfPackage<=0){
    printf("包裹重量不合法！请重新输入：\n");    //防止输入为负数或0
    scanf("%f", &weightOfPackage);

  }
  newPackage->weight=weightOfPackage;
  
  if(weightOfPackage<=3){
    costOfMoney+=10;
  }else{
    costOfMoney+=(10+(weightOfPackage-3)*1);
  }
  
  //包装类型
  printf("请输入需要的包装物料类型：\n1.邮件（1元） 2.小包裹（2元） 3.中包裹（5元） 4.大包裹（10元） 5.特大包裹（15元）\n");
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
        printf("请输入正确的数字以选择包裹物料类型！请重新输入：\n");
        scanf("%d", &choice);
        while(getchar()!='\n');
      }
    }
  }
  newPackage->volume = PackageSizeToVolume(newPackage->packageSize);
  
  //新增取件到楼服务
  int serviceChoice;
  printf("是否需要取件到楼服务？请输入数字以选择：\n1.是 2.否\n");
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
        int timeIsLegal=0;
        while(!timeIsLegal){
          // 读取用户输入（格式：月/日 时:分）
          int month, day, hour, minute;
          printf("请输入一个您方便的时间，我们将于那时上门取件（格式 MM/DD hh:mm）：\n");
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
            while(getchar()!='\n');
            continue;
          }

          // 比较时间
          if (target_time == -1) {
            printf("错误：无效的日期！\n");
            while(getchar()!='\n');
            continue;
          } else if (target_time < now) {
            printf("输入的时间已过。\n");
            while(getchar()!='\n');
            continue;
          } else {
            printf("预约成功！\n");
            timeIsLegal=1;
            while(getchar()!='\n');
            continue;
          }
        }
        getchar();
        char targetAddress[AddressLen];
        if(StrInputValidation("您希望的取件地址", AddressLen, 0, targetAddress)){
          return;
        }
        choiceIsPass=1;
        break;
      }
      case 2:{
        choiceIsPass=1;
        break;
      }
      default:{               //若输入非法则循环到获得符合要求的输入
        printf("请输入1或2以选择：\n");
        scanf("%d", &serviceChoice);
        while(getchar()!='\n');
      }
    }
  }
  
  
  //新用户有六次优惠
  if(user->numOfDiscount){
    if(user->userType==student){
      costOfMoney*=0.7f;
    }else if(user->userType==teacher){
      costOfMoney*=0.5f;
    }
    user->numOfDiscount--;
  }
  //活动满减（每满20减2）
  printf("您被发件优惠活动砸中！运费每满20元减2元！您的包裹优惠前运费为：%.2f，", costOfMoney);
  costOfMoney-=(int)(costOfMoney/20)*2;

  newPackage->fee=costOfMoney;
  printf("优惠后运费为：%.2f元，单击回车以返回\n", costOfMoney);
  getchar();
  while(getchar()!='\n');
  
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
  
  addParcelToInventory(inventory, newPackage);                    //向待寄区加入包裹

  return;
}

void searchParcelInterface(UserData* user, PackageData* packageList){
  PackageData* tmpPtr=packageList;
  int cnt=0;
  printf("这些包裹可能是您的：\n");
  while(tmpPtr!=NULL){
    if(strcmp(user->name, tmpPtr->name) == 0 && tmpPtr->packageStatus==pendingPickup){
      printf("%s\n", tmpPtr->pickUpCode);
      cnt++;
    }
    tmpPtr=tmpPtr->nextPackageData;
  }
  if(!cnt){
    printf("\n**暂无待取包裹**\n");
  }
  tmpPtr=NULL;
  free(tmpPtr);
  return;
}

void getParcelFromInventory(UserData* user, PackageData* packageList, InventoryManagement* inventory){
  char targetPickUpCode[PickupCodeLen];
  if(StrInputValidation("取件码", PickupCodeLen, 1, targetPickUpCode)){
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
    printf("错误的取件码！单击回车以返回\n");
    while(getchar()!='\n');
    return;
  }else{
    printf("找到包裹！请输入您的密码以取走包裹!\n");
    char targetPassword[PinLen];
    if(StrInputValidation("您的密码", PinLen, 1, targetPassword)){
      return;
    }
    int passwordIsCorrect=0;
    while(!passwordIsCorrect){
      if(strcmp(targetPassword, user->pin) == 0){
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
  //新增取件到楼服务
  int serviceChoice;
  printf("是否需要送件到楼服务？请输入数字以选择：\n1.是 2.否\n");
  scanf("%d", &serviceChoice);
  int choiceIsPass=0;
  while(!choiceIsPass){
    switch (serviceChoice){
      case 1:{
        //预约取件时间
        time_t now;
        time(&now); // 获取当前时间戳
        int timeIsLegal=0;
        while(!timeIsLegal){
          // 读取用户输入（格式：月/日 时:分）
          int month, day, hour, minute;
          printf("请输入一个您方便的时间，我们将于那时上门送件（格式 MM/DD hh:mm）：\n");
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
            while(getchar()!='\n');
            continue;
          }

          // 比较时间
          if (target_time == -1) {
            printf("错误：无效的日期！\n");
            while(getchar()!='\n');
            continue;
          } else if (target_time < now) {
            printf("输入的时间已过。\n");
            while(getchar()!='\n');
            continue;
          } else {
            printf("预约成功！\n");
            timeIsLegal=1;
            while(getchar()!='\n');
            continue;
          }
        }
        getchar();
        char targetAddress[AddressLen];
        if(StrInputValidation("您希望的送件地址", AddressLen, 0, targetAddress)){
          return;
        }
        choiceIsPass=1;
        break;
      }
      case 2:{
        choiceIsPass=1;
        break;
      }
      default:{               //若输入非法则循环到获得符合要求的输入
        printf("请输入1或2以选择：\n");
        scanf("%d", &serviceChoice);
        while(getchar()!='\n');
      }
    }
  }

  tmpPtr=NULL;
  free(tmpPtr);
  return;
}
//取消寄件
void cancelSending(UserData* user, PackageData** packageList, InventoryManagement* inventory){
  if (*packageList == NULL) {
    printf("未查找到包裹记录\n");
    while(getchar()=='\n')return;
  }

  PackageData* current = *packageList;
  int found = 0;

  char targetPickUpCode[PickupCodeLen];
  StrInputValidation("您要取消的包裹的运单号", PickupCodeLen, 1, targetPickUpCode);

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
    printf("未找到该包裹\n");
    while(getchar()=='\n')return;
  }
  printf("成功取消寄件！请及时取走您的包裹\n");
  while(getchar()=='\n')return;
}
//查询某位用户的历史取出或寄出包裹并分类显示
void displayUserHistory(UserData* user, PackageData* packageList){
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
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==pendingPickup){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("已签收：\n");
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
        printf("未寄出：\n");
        while(tmpPtr!=NULL){
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==pendingSend){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("已寄出：\n");
        while(tmpPtr!=NULL){
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==sent){
            fputs(tmpPtr->pickUpCode, stdout);
            printf("\n");
          }
          tmpPtr=tmpPtr->nextPackageData;
        }
        tmpPtr=packageList;
        printf("已取消：\n");
        while(tmpPtr!=NULL){
          if(strcmp(tmpPtr->name, user->name) == 0 && tmpPtr->packageStatus==canceled){
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

void modifyUserProfile(UserData* user){
  int choice=0;
  int choiceIsPass=0;
  printf("请输入数字以选择您要修改的信息：\n1.用户名 2，密码\n");
  scanf("%d", &choice);
  getchar();
  while(!choiceIsPass){
    switch (choice){
      case 1:{
        choiceIsPass=1;
        if(StrInputValidation("您的新用户名",UserNameLen,0,user->userName)){
          return;
        }
        printf("您的用户名修改成功！\n");
        break;
      }
      case 2:{
        choiceIsPass=1;
        char newPassword[PinLen], newPasswordConfirmer[PinLen];
        if(StrInputValidation("您的新密码",PinLen,1,newPassword)){
          return;
        }
        if(StrInputValidation("您的新密码以确认",PinLen,1,newPasswordConfirmer)){
          return;
        }
        int confirmer=0;
        while(!confirmer){
          if(strcmp(newPassword, newPasswordConfirmer)==0){
            confirmer=1;
            newPassword[strcspn(newPassword, "\n")]='\0';
            strcpy(user->pin, newPassword);
            printf("您的密码修改成功！\n");
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
//展示当前用户信息
void displayUserInformation(UserData* user){
  printf("实名信息：%s\n", user->name);
  printf("账号：%s\n", user->userName);
  printf("用户类型：%s\n", UserTypeToStr(user->userType));
  printf("密码：%s\n", user->pin);
  printf("新用户优惠剩余次数：%d\n", user->numOfDiscount);
  getchar();
}