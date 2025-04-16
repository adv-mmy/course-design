#include"data_storage.h"
#include"str_and_enum.h"

bool NameStorage(char* name,UserData* p){//�������溯��
if(strlen(name)>=NameLen)//����һ��Ŀ���Դ�ַ������ȣ���������flase��flase����ô�������Լ����Ű�
    return false;
strcpy(p->name,name);
return true;
}


bool UserNameStorage(char* userName,UserData* p){//�û��ǳƴ��溯��
if(strlen(userName)>=UserNameLen)
    return false;
strcpy(p->userName,userName);
return true;
}


bool PinStorage(char* pin,UserData* p){//���봢�溯��
if(strlen(pin)>=PinLen)
    return false;
strcpy(p->pin,pin);
return true;
}


bool UserTypeStorage(char* userType,UserData* p){//�û����ʹ��溯��
for(int i=0;i<3;i++){
if(strcmp(userType,userTypeStr[i])==0){
    p->userType=StrToUserType(userType);
    return true;}}                       //����һ���û�������ַ����Ƿ���ȷ������ȷ�Ļ�����false
return false;
}


bool PermissionStorage(bool permission,UserData* p){//�û�Ȩ�޴��溯�����������ûʲô���ݣ�д��������Ϊ���õ�ʱ��ɶ��Ը���
p->permission=permission;
return true;
}


bool NumOfDiscountStorage(int numOfDiscount,UserData* p){//�Żݴ����Ĵ��溯��
if(numOfDiscount>5||numOfDiscount<0)
    return false;
p->numOfDiscount=numOfDiscount;
return true;
}


bool VolumeStorage(float volume,PackageData* p){//��������Ĵ���
float intpart=volume*100;
if(volume<0.0||volume*100-intpart!=0)   //�������������Ƿ���һ����λС����������λС���ͷ���false�����洢
    return false;
p->volume=volume;
return true;
}


bool WeightStorage(float weight,PackageData* p){//���������Ĵ���
float intpart=weight*100;
if(weight<0.0||weight*100-intpart!=0)   //�������������Ƿ���һ����λС����������λС���ͷ���false�����洢
    return false;
p->weight=weight;
return true;

}


bool FeeStorage(float fee,PackageData* p){//����Ӧ�����õĴ���
int intpart=fee*100;
if(fee<0.0||fee*100-intpart!=0)   //����һ�·����ǲ�����λС����Ҫ��С��λ�����˾ͷ���flase
    return false;                 //����Ϊ�����������û����۵�Ե�ʣ�Ҫ�ǳ�������ʱ��������λС���Ļ�������д����������֮��ĵķź�������              
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
if(strlen(pickUpCode)!=(PickupCodeLen-1))     //����ȡ֤��ĳ��ȣ����Ⱦ���4����������
    return false;    
return true;                         
}


bool PackageTypeStorage(char* packageType,PackageData* p){
for(int i=0;i<2;i++){
if(strcmp(packageType,packageTypeStr[i])==0){
    p->packageType=StrToPackageType(packageType);
    return true;}}                     //����һ���û�������ַ����Ƿ���ȷ������ȷ�Ļ�����false
return false;
}


bool PackageStatusStorage(char* packageStatus,PackageData* p){
for(int i=0;i<4;i++){
    if(strcmp(packageStatus,packageStatusStr[i])==0){
        p->packageStatus=StrToPackageStatus(packageStatus);
        return true;}}                //����һ���û�������ַ����Ƿ���ȷ������ȷ�Ļ�����false
return false;
}


bool PackageSize(char* packageSize,PackageData* p){
for(int i=0;i<5;i++){
    if(strcmp(packageSize,packageSizeStr[i])==0){
        p->packageSize=StrToPackageSize(packageSize);
        return true;}}                       //����һ���û�������ַ����Ƿ���ȷ������ȷ�Ļ�����false
return false;
}


//�����ֱ��������ַ������ƣ����Ƴ��ȣ��Ƿ������֣�Ŀ���ַ�����ַ
bool StrInputValidation(const char* InputType,int InputLenth,int ValidationType,char* StrDestination){
    printf("������%s������-1�Է��أ���\n" ,InputType);
    char inputStr[1024];
            fgets(inputStr, 1024, stdin);
    switch (ValidationType){
    case 1:
        while(1){
            if(strcmp(inputStr,"-1\n")==0){
                printf("���»س��Է���");
                getchar();
                while(getchar()!='\n');
                return true;
            }
            int flag=0;
            if(inputStr[0]=='\n'){//<-���������
                printf("��������أ�Ϊʲô�ҿ�������\n");
                printf("���������룺\n");
                fgets(inputStr, 1024, stdin);         
                continue;
            }
            inputStr[strcspn(inputStr, "\n")] = '\0';

            if(strlen(inputStr)>=InputLenth){//<-�������볤��
                printf("����%s̫���ˣ�����ǲ�סŶo.0\n",InputType);
                printf("���������룺\n");
                fgets(inputStr, 1024, stdin);
                continue;
            }

            for(int i=0;inputStr[i]!='\0';i++){//<-������������
                if(inputStr[i]<=33||inputStr[i]>=126){
                    printf("�벻Ҫ����ȫ���ַ������Ļ�ո񣬳��򿴲�����:->");
                    printf("���������룺\n");
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
                printf("���»س��Է���");
                getchar();
                while(getchar()!='\n');
                return true;
            }
            if(strlen(inputStr)>=InputLenth){//<-�������볤��
                printf("����%s̫���ˣ�����ǲ�סŶo.0\n",InputType);
                printf("���������룺\n");
                fgets(inputStr,1024, stdin);
                continue;
            }

            if(inputStr[0]=='\n'){//<-���������
                printf("��������أ�Ϊʲô�ҿ�������\n");
                printf("���������룺\n");
                fgets(inputStr, 1024, stdin);         
                continue;
            }
            inputStr[strcspn(inputStr, "\n")] = '\0';

            strcpy(StrDestination,inputStr);
            break;
        }
    }
    return false;//����-1���ؼ�
}