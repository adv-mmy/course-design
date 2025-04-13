#include"common_value.h"
#include<string.h>
#include"str_and_enum.h"
const char* userTypeStr[] = {"student", "teacher", "others"};
const char* packageTypeStr[] = {"toPickup", "toSend"};
const char* packageStatusStr[] = {"pendingPickup", "pickedUp", "pendingSend", "sent"};
const char* packageSizeStr[] = {"pail", "small", "medium", "large", "extraLarge"};
/*�����ַ���תö�����͵ĺ����������ת�����ַ���û�����趨�Ķ�Ӧ���᷵��wrongX
*X��Ӧ�������������������Ӧ�ĵ�ö�����͵ı����е�ֵ��ö������û�г�ʼ����һ��Ԫ��Ϊ0����n��Ԫ��Ϊn-1��
*/
const char* UserTypeToStr(enum UserType userType) {
    return userTypeStr[userType];
}

enum UserType StrToUserType(const char* str) {
    for(int i=0; i<3; i++)
        if(strcmp(str, userTypeStr[i]) == 0) 
        return (enum UserType)i;
    return (enum UserType)wrong3;
}

const char* PackageTypeToStr(enum PackageType type) {
    return packageTypeStr[type];
}

enum PackageType StrToPackageType(const char* str) {
    for(int i=0; i<2; i++)
        if(strcmp(str, packageTypeStr[i]) == 0) 
        return (enum PackageType)i;
    return (enum PackageType)wrong2;
}

const char* PackageStatusToStr(enum PackageStatus status) {
    return packageStatusStr[status];
}

enum PackageStatus StrToPackageStatus(const char* str) {
    for(int i=0; i<4; i++)
        if(strcmp(str, packageStatusStr[i]) == 0) 
        return (enum PackageStatus)i;
    return (enum PackageStatus)wrong4;
}

const char* PackageSizeToStr(enum PackageSize size) {
    return packageSizeStr[size];
}

enum PackageSize StrToPackageSize(const char* str) {
    for(int i=0; i<5; i++)
        if(strcmp(str, packageSizeStr[i]) == 0) 
        return (enum PackageSize)i;
    return (enum PackageSize)wrong5;
}