#ifndef STR_AND_ENUM_H
#define STR_AND_ENUM_H
#include"common_value.h"

const char* UserTypeToStr(enum UserType type);
enum UserType StrToUserType(const char* str);
const char* PackageTypeToStr(enum PackageType type);
enum PackageType StrToPackageType(const char* str);
const char* PackageStatusToStr(enum PackageStatus status);
enum PackageStatus StrToPackageStatus(const char* str);
const char* PackageSizeToStr(enum PackageSize size);
enum PackageSize StrToPackageSize(const char* str);

#endif 