#ifndef COMMON_VALUE_H
#define COMMON_VALUE_H
#define NameLen         11   
#define UserNameLen     21    
#define PinLen          21
#define TimeLen         11  
#define LocationLen     51  
#define UserPermission  0    
#define AdminPermission 1     
#define MaxPackages 2000 
#define PickupCodeLen 6 
#define NumOfShelves 50
#define ShelvesLen 2 //对于字符串长度的定义其实应该是定义的长度减1，但是字符串要存一个'/0'
#include <stdbool.h>

extern const char* userTypeStr[] ;
extern const char* packageTypeStr[] ;
extern const char* packageStatusStr[];
extern const char* packageSizeStr[] ;
/*声明全局变量，防止引用不同头文件到主函数时造成多次定义报错*/

enum PackageType{      //包裹类型
    toPickup,          //取件
    toSend,            //寄件
    wrong2
} ;

enum PackageStatus{       //包裹当前状态
    pendingPickup,        //待取
    pickedUp,             //已取
    pendingSend,          //待寄
    sent,                 //已寄;
    wrong4
};

/*typedef enum{           //结费方式
    CushPay,              //现金
    OnlinePay,            //线上付款
}  FeeMethod;*/

enum PackageSize{         //包装类型
    mail,                 //邮件
    small,                //小包装
    medium,               //中包装
    large,                //大包装
    extraLarge,           //特大包装
    wrong5
};

 enum UserType { student, teacher, others, wrong3 };  


typedef struct Userdata{
    char name[NameLen];           
    char userName[UserNameLen];    //区分这两个变量，name是真实姓名,userName是账号名，仅用于登录
    char pin[PinLen];              
    bool permission;
    enum UserType userType;         
    int numOfDiscount;
    struct Userdata* nextUserData;  
    struct PackageData* nextPackageData;

} UserData;

typedef struct PackageData{
    float volume;  
    char name[NameLen];           
    float weight;
    char inTime[TimeLen];        //202X.XX.XX
    float fee;
    char location[LocationLen];
    char pickUpCode[PickupCodeLen];
    enum PackageType packageType;
    enum PackageStatus packageStatus;
    enum PackageSize packageSize;
    struct PackageData* nextPackageData;
    
}PackageData;
#endif

/*写几个包裹和个人信息的范例，你们还能看明白
@U John  johndoe  pass123  0  student  3      // 用户数据 以@U(大写)开头做标志
@P 2.3  A  15.0  truck  ABC1  toPickup  pendingPickup  small  // 包裹1  @P(大写)开头做标志
@P 5.0  B  20.0  plane  XYZ2  toSend  pent  medium // 包裹2  标志同上                                                            // 空行
@U Alice  alice123  qwerty  1  teacher  1    // 下一个用户
按照    用户信息——>用户包裹信息———>用户包裹信息   下一个用户信息       的方式储存在文件里
*/
