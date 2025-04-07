#ifndef DATA_STOARGE_H
#define DATA_STOARGE_H
#include<string.h>
#include"common_value.h"
#include"str_and_enum.h"

bool NameStorage(char* name,UserData* p);
bool UserNameStorage(char* userName,UserData* p);
bool PinStorage(char* pin,UserData* p);
bool NumOfDiscountStorage(int numOfDis,UserData* p);
bool UserTypeStorage(char* userType,UserData* p);
bool PermissionStorage(bool permission,UserData* p);
//bool VolumndStorage(float volume,PackageData* p);
bool WeightStorage(float weight,PackageData* p);
bool InTimeStorage(char* inTime,PackageData* p);
bool FeeStorage(float fee,PackageData* p);
//bool TransportModeStorage(char* transportMode,PackageData* p);
bool LocationStorage(char* location,PackageData* p);
bool PickUpCodeStorage(char* pickUpCode,PackageData* p);
bool PackageTypeStorage(char*  PackageType,PackageData* p);
bool PackageStatusStorage(char*  PackageStatus,PackageData* p);
bool PackageSize(char*  PackageSize,PackageData* p);

#endif