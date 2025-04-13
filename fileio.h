// File: file_operations.h
#ifndef FILE_IO_H
#define FILE_IO_H

#include "common_value.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "str_and_enum.h"

// �ļ�д�뺯��
bool WriteUserToFile(FILE* fp, const UserData* user);
bool WritePackageToFile(FILE* fp, const PackageData* package);

// �ļ���ȡ����
bool ReadUserFromFile(FILE* fp, UserData* user);
bool ReadPackageFromFile(FILE* fp, PackageData* package);

//���ת������
float PackageSizeToVolume(enum PackageSize);
#endif