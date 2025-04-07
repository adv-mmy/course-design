// File: file_operations.c
/*

写函数比较简单，一次只写入单独一个用户信息或者单独一个包裹信息
读函数也有一次只单独读一个用户信息或者一个包裹信息，最后那个函数是用来直接读把一个人的所有信息都读入的
包裹信息会被顺序存在一个地址下的n*sizeof（PackageData）的空间里，看你怎么用了。这种储存方式暂定
当然，你也可以单独用一个malloc来进行空间的分配写链表，或者干脆不用这个函数，只把读其余两个读函数当作最小单元进行使用
*/
#include "fileio.h"
#include <string.h>
#include <stdlib.h>
#include "str_and_enum.h"
const int volume[]={0.01,0.2,0.5,1,2};
//体积转换函数，这个函数本来应该写在另一个头文件的，偷懒写在这儿吧
float PackageSizeToVolume(enum PackageSize package_size){
    if(package_size>4)
        return -1.0;//如果输入的值不是一个合法的enum数组就返回-1
    return volume[package_size];

}



// 写入函数
bool WriteUserToFile(FILE* fp, const UserData* user) {
    return fprintf(fp, "@U  %-*s  %-*s  %-*s  %d  %s  %d\n",
        NameLen-1, user->name,
        UserNameLen-1, user->userName,
        PinLen-1, user->pin,
        user->permission,
        UserTypeToStr(user->userType),
        user->numOfDiscount) > 0;
}

bool WritePackageToFile(FILE* fp, const PackageData* package) {
    return fprintf(fp, "@P  %.2f  %-*s  %-*s  %.2f  %-*s  %-*s  %s  %s  %s\n",
    //    package->volume,
        package->weight,
        NameLen-1, package->name,
        TimeLen-1 , package->inTime,
        package->fee,
        LocationLen-1,package->location,
    //    package->transportMode,
        PickupCodeLen-1, package->pickUpCode,
        PackageTypeToStr(package->packageType),
        PackageStatusToStr(package->packageStatus),
        PackageSizeToStr(package->packageSize)) > 0;
}

// 读取函数
bool ReadUserFromFile(FILE* fp, UserData* user) {
    char type_str[20];
    int result = fscanf(fp,"%10s %20s %20s %d %20s %d",
        user->name,
        user->userName,
        user->pin,
        &user->permission,
        type_str,
        &user->numOfDiscount);
    if(result != 6) return false;
        user->userType = StrToUserType(type_str);
    return true;
}

bool ReadPackageFromFile(FILE* fp, PackageData* package) {
    char type_str[20], status_str[20], size_str[20];
    int result = fscanf(fp,"%f %10s %10s %f %5s %s %s %s",
        //&package->volume,
        &package->name,
        &package->weight,
        &package->inTime,
        &package->fee,
    //    package->transportMode,
        &package->pickUpCode,
        type_str,
        status_str,
        size_str);
    if(result != 8) return false;
    package->packageType = StrToPackageType(type_str);
    package->packageStatus = StrToPackageStatus(status_str);
    package->packageSize = StrToPackageSize(size_str);
    package->volume = PackageSizeToVolume(package->packageSize);
    return true;
}
