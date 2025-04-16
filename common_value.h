#ifndef COMMON_VALUE_H
#define COMMON_VALUE_H

#define NameLen         31   
#define UserNameLen     31   
#define PinLen          21
#define TimeLen         11  
#define AddressLen      51  
#define UserPermission  0    
#define AdminPermission 1     
#define MaxPackages 2000 
#define PickupCodeLen 9 
#define NumOfShelves 50
#define ShelvesLen 2 //�����ַ������ȵĶ�����ʵӦ���Ƕ���ĳ��ȼ�1�������ַ���Ҫ��һ��'/0'

#include <stdbool.h>

extern const char* userTypeStr[] ;
extern const char* packageTypeStr[] ;
extern const char* packageStatusStr[];
extern const char* packageSizeStr[] ;
/*����ȫ�ֱ�������ֹ���ò�ͬͷ�ļ���������ʱ��ɶ�ζ��屨��*/

enum PackageType{      //��������
    toPickup,          //ȡ��
    toSend,            //�ļ�
    wrong2
} ;

enum PackageStatus{       //������ǰ״̬
    pendingPickup,        //��ȡ
    pickedUp,             //��ȡ
    pendingSend,          //����
    sent,                 //�Ѽ�;
    wrong4
};

enum PackageSize{         //��װ����
    mail,                 //�ʼ�
    small,                //С��װ
    medium,               //�а�װ
    large,                //���װ
    extraLarge,           //�ش��װ
    wrong5
};

 enum UserType { student, teacher, others, wrong3 };  


typedef struct Userdata{
    char name[NameLen];           
    char userName[UserNameLen];    //����������������name����ʵ����,userName���˺����������ڵ�¼
    char pin[PinLen];              
    bool permission;
    enum UserType userType;         
    int numOfDiscount;
    struct Userdata* nextUserData;  
} UserData;

typedef struct PackageData{
    float volume;  
    char name[NameLen];           
    float weight;
    float fee;
    char address[AddressLen];
    char pickUpCode[PickupCodeLen];      //��ȡ�����������ȡ���룬���İ���������ǵ���
    enum PackageType packageType;
    enum PackageStatus packageStatus;
    enum PackageSize packageSize;
    struct PackageData* nextPackageData;
    
}PackageData; 
#endif

/*д���������͸�����Ϣ�ķ��������ǻ��ܿ�����
 John  johndoe  pass123  0  student  3      // �û����� ��@U(��д)��ͷ����־
 John 2.3  2024.02.29 15.0  ������԰  ABC1  toPickup  pendingPickup  small  
 2.3  A  15.0  truck  ABC1  toPickup  pendingPickup  small  // ����1  @P(��д)��ͷ����־
 5.0  B  20.0  plane  XYZ2  toSend  pent  medium // ����2  ��־ͬ��                                                            // ����
������Ϣ   ��һ���û���Ϣ       �ķ�ʽ�������ļ���
*/
