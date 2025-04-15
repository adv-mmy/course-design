#include"data_storage.h"
#include"str_and_enum.h"

bool NameStorage(char* name,UserData* p){//姓名储存函数
if(strlen(name)>=NameLen)//检验一下目标和源字符串长度，超长返回flase，flase后怎么操作你自己看着办
    return false;
strcpy(p->name,name);
return true;
}


bool UserNameStorage(char* userName,UserData* p){//用户昵称储存函数
if(strlen(userName)>=UserNameLen)
    return false;
strcpy(p->userName,userName);
return true;
}


bool PinStorage(char* pin,UserData* p){//密码储存函数
if(strlen(pin)>=PinLen)
    return false;
strcpy(p->pin,pin);
return true;
}


bool UserTypeStorage(char* userType,UserData* p){//用户类型储存函数
for(int i=0;i<3;i++){
if(strcmp(userType,userTypeStr[i])==0){
    p->userType=StrToUserType(userType);
    return true;}}                       //检验一下用户输入的字符串是否正确，不正确的话返回false
return false;
}


bool PermissionStorage(bool permission,UserData* p){//用户权限储存函数，这个函数没什么内容，写出来就是为了用的时候可读性更高
p->permission=permission;
return true;
}


bool NumOfDiscountStorage(int numOfDiscount,UserData* p){//优惠次数的储存函数
if(numOfDiscount>5||numOfDiscount<0)
    return false;
p->numOfDiscount=numOfDiscount;
return true;
}


bool VolumeStorage(float volume,PackageData* p){//包裹体积的储存
float intpart=volume*100;
if(volume<0.0||volume*100-intpart!=0)   //检验输入的体积是否是一个两位小数，不是两位小数就返回false，不存储
    return false;
p->volume=volume;
return true;
}


bool WeightStorage(float weight,PackageData* p){//包裹重量的储存
float intpart=weight*100;
if(weight<0.0||weight*100-intpart!=0)   //检验输入的体积是否是一个两位小数，不是两位小数就返回false，不存储
    return false;
p->weight=weight;
return true;

}


bool FeeStorage(float fee,PackageData* p){//包裹应付费用的储存
int intpart=fee*100;
if(fee<0.0||fee*100-intpart!=0)   //检验一下费用是不是两位小数，要是小数位数超了就返回flase
    return false;                 //（因为有满减和新用户打折的缘故，要是程序计算的时候会产生三位小数的话就让我写个四舍五入之类的的放函数里面              
p->fee=fee;
return true;
}


bool AddressStorage(char* address,PackageData* p){
if(strlen(address)>=AddressLen)
    return false;
strcpy(p->address,address);
return true;
}


bool PickUpCodeStorage(char* pickUpCode,PackageData* p){
if(strlen(pickUpCode)!=(PickupCodeLen-1))     //检验取证码的长度，长度就是4，你别多输了
    return false;    
return true;                         
}


bool PackageTypeStorage(char* packageType,PackageData* p){
for(int i=0;i<2;i++){
if(strcmp(packageType,packageTypeStr[i])==0){
    p->packageType=StrToPackageType(packageType);
    return true;}}                     //检验一下用户输入的字符串是否正确，不正确的话返回false
return false;
}


bool PackageStatusStorage(char* packageStatus,PackageData* p){
for(int i=0;i<4;i++){
    if(strcmp(packageStatus,packageStatusStr[i])==0){
        p->packageStatus=StrToPackageStatus(packageStatus);
        return true;}}                //检验一下用户输入的字符串是否正确，不正确的话返回false
return false;
}


bool PackageSize(char* packageSize,PackageData* p){
for(int i=0;i<5;i++){
    if(strcmp(packageSize,packageSizeStr[i])==0){
        p->packageSize=StrToPackageSize(packageSize);
        return true;}}                       //检验一下用户输入的字符串是否正确，不正确的话返回false
return false;
}


//参数分别是输入字符串名称，控制长度，是否允许汉字，目标字符串地址
bool StrInputValidation(const char* InputType,int InputLenth,int ValidationType,char* StrDestination){
    printf("请输入%s（输入-1以返回）：\n" ,InputType);
    char inputStr[1024];
            fgets(inputStr, 1024, stdin);
    switch (ValidationType){
    case 1:
        while(1){
            if(strcmp(inputStr,"-1\n")==0){
                printf("按下回车以返回");
                getchar();
                while(getchar()!='\n');
                return true;
            }
            int flag=0;
            if(inputStr[0]=='\n'){//<-空输入检验
                printf("你的输入呢？为什么我看不到？\n");
                printf("请重新输入：\n");
                fgets(inputStr, 1024, stdin);         
                continue;
            }
            inputStr[strcspn(inputStr, "\n")] = '\0';

            if(strlen(inputStr)>=InputLenth){//<-检验输入长度
                printf("您的%s太长了，程序记不住哦o.0\n",InputType);
                printf("请重新输入：\n");
                fgets(inputStr, 1024, stdin);
                continue;
            }

            for(int i=0;inputStr[i]!='\0';i++){//<-检验中文输入
                if(inputStr[i]<=33||inputStr[i]>=126){
                    printf("请不要输入全角字符，中文或空格，程序看不懂捏:->");
                    printf("请重新输入：\n");
                    fgets(inputStr, 1024, stdin);
                    flag=1;
                    break;
                }
            }
            if(flag==1)
                continue;
            
            strcpy(StrDestination,inputStr);
            break;
        }
        break;
    default:
        while(1){
            if(strcmp(inputStr,"-1\n")==0){
                printf("按下回车以返回");
                getchar();
                while(getchar()!='\n');
                return true;
            }
            if(strlen(inputStr)>=InputLenth){//<-检验输入长度
                printf("您的%s太长了，程序记不住哦o.0\n",InputType);
                printf("请重新输入：\n");
                fgets(inputStr,1024, stdin);
                continue;
            }

            if(inputStr[0]=='\n'){//<-空输入检验
                printf("你的输入呢？为什么我看不到？\n");
                printf("请重新输入：\n");
                fgets(inputStr, 1024, stdin);         
                continue;
            }
            inputStr[strcspn(inputStr, "\n")] = '\0';

            strcpy(StrDestination,inputStr);
            break;
        }
    }
    return false;//不是-1返回假
}