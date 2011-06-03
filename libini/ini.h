#ifndef _INI_H__
#define _INI_H__
#include <stdlib.h>
#include <stdio.h>

typedef struct ini_item_t {
	struct ini_item_t *prev;
	struct ini_item_t *next;
	char *name;
	char *value;
} INIITEM, *LPINIITEM;

typedef struct ini_section_t {
	struct ini_section_t *prev;
	struct ini_section_t *next;
	char *name;
	int itemCount;
	struct ini_item_t *first_item;
	struct ini_item_t *last_item;
} INISECTION, *LPINISECTION;

typedef struct ini_file_t {
	char *name;
	int sectionCount;
	struct ini_section_t *first_section;
	struct ini_section_t *last_section;
	int flag;
	char mode[8];
} INIFILE, *LPINIFILE;

// INI句柄
typedef void* INIFILEHANDLE;

// 将ini文件另存到一个I/O流
int ini_SaveTo(INIFILEHANDLE ini, FILE *fp);
// 将ini文件另存为
int ini_saveAs(INIFILEHANDLE ini, const char *saveFile);
// 关闭ini文件
void ini_close(INIFILEHANDLE ini);
// 从ini文件读取一个字符串类型的属性值
char *ini_ReadString(INIFILEHANDLE ini, const char *secName, const char *itemName, char *outBuf, const char *Default);
// 从ini文件读取一个整数类型的属性值
int ini_ReadInt(INIFILEHANDLE ini, const char *secName, const char *itemName, int Default);
// 向ini文件写入一个字符串类型的属性值,如果ini文件没有该属性,则创建
int ini_WriteString(INIFILEHANDLE ini, const char *secName, const char *itemName, const char *value);
// 向ini文件写入一个整数类型的属性值,如果ini文件没有该属性,则创建
int ini_WriteInt(INIFILEHANDLE ini, const char *secName, const char *itemName, int Value);
//additem
LPINIITEM ini_addItem(LPINISECTION iniSection, const char *name, const char *value);
//delitem
void ini_delItem(LPINISECTION iniSection, LPINIITEM item);
//add section
LPINISECTION ini_addSection(LPINIFILE iniFile, const char *name);
//del section
void ini_delSection(LPINIFILE iniFile, LPINISECTION sec);

// 打开一个ini文件
//    iniFile: ini文件路径
//    mode: 打开方式, 与fopen的第二个参数意义相同
INIFILEHANDLE ini_open(const char *iniFile, const char *mode);


//C格式到EFI格式转换函数
char* a2u(char*str);
//获得ini的label个数
int secNumbers(LPINIFILE ini);
//获得label的item个数
int itemNumbers(LPINISECTION sec);
#endif//_INI_H__
