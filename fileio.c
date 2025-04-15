// File: file_operations.c
/*

д�����Ƚϼ򵥣�һ��ֻд�뵥��һ���û���Ϣ���ߵ���һ��������Ϣ
������Ҳ��һ��ֻ������һ���û���Ϣ����һ��������Ϣ������Ǹ�����������ֱ�Ӷ���һ���˵�������Ϣ�������
������Ϣ�ᱻ˳�����һ����ַ�µ�n*sizeof��PackageData���Ŀռ��������ô���ˡ����ִ��淽ʽ�ݶ�
��Ȼ����Ҳ���Ե�����һ��malloc�����пռ�ķ���д�������߸ɴ಻�����������ֻ�Ѷ���������������������С��Ԫ����ʹ��
*/
#include "fileio.h"
#include <string.h>
#include <stdlib.h>
#include "str_and_enum.h"
const float volume[]={0.01f,0.2f,0.5f,1.0f,2.0f};
//���ת�������������������Ӧ��д����һ��ͷ�ļ��ģ�͵��д�������
float PackageSizeToVolume(enum PackageSize package_size){
    if(package_size>4)
        return -1.0;//��������ֵ����һ���Ϸ���enum����ͷ���-1
    return volume[package_size];

}


// д�뺯��
bool WriteUserToFile(FILE* fp, const UserData* user) {
    return fprintf(fp, "%-*s  %-*s  %-*s  %d  %s  %d\n",
        NameLen-1, user->name,
        UserNameLen-1, user->userName,
        PinLen-1, user->pin,
        user->permission,
        UserTypeToStr(user->userType),
        user->numOfDiscount) != -1;
}

bool WritePackageToFile(FILE* fp, const PackageData* package) {
    return fprintf(fp, "%-*s  %.2f  %.2f  %-*s  %-*s  %s  %s  %s\n",
        NameLen-1, package->name,
        package->weight,
        package->fee,
        AddressLen-1,package->address,
        PickupCodeLen-1, package->pickUpCode,
        PackageTypeToStr(package->packageType),
        PackageStatusToStr(package->packageStatus),
        PackageSizeToStr(package->packageSize)) != -1;
}

bool WriteInventoryStatusToFile(FILE* fp, const ShelfNode* shelf){
    return fprintf(fp, "%d   %.2f\n",
        shelf->packageCnt,
        shelf->remainSpace)==2;
}
// ��ȡ����
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
    int result = fscanf(fp,"%10s %f %f %50s %5s %s %s %s",
        package->name,
        &package->weight,
        &package->fee,
        package->address,
        package->pickUpCode,
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

bool ReadInventoryStatusFromFile(FILE* fp, ShelfNode* shelf){
    int result = fscanf(fp, "%d   %f",
        &shelf->packageCnt,
        &shelf->remainSpace);
    if(result != 2) return false;
    return true;
}
