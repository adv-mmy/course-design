// File: file_operations.h
#ifndef FILE_IO_H
#define FILE_IO_H

#include "common_value.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "str_and_enum.h"

// 文件写入函数
bool WriteUserToFile(FILE* fp, const UserData* user);
bool WritePackageToFile(FILE* fp, const PackageData* package);

// 文件读取函数
bool ReadUserFromFile(FILE* fp, UserData* user);
bool ReadPackageFromFile(FILE* fp, PackageData* package);

//体积转换函数
float PackageSizeToVolume(enum PackageSize);
#endif