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

// INI���
typedef void* INIFILEHANDLE;

// ��ini�ļ���浽һ��I/O��
int ini_SaveTo(INIFILEHANDLE ini, FILE *fp);
// ��ini�ļ����Ϊ
int ini_saveAs(INIFILEHANDLE ini, const char *saveFile);
// �ر�ini�ļ�
void ini_close(INIFILEHANDLE ini);
// ��ini�ļ���ȡһ���ַ������͵�����ֵ
char *ini_ReadString(INIFILEHANDLE ini, const char *secName, const char *itemName, char *outBuf, const char *Default);
// ��ini�ļ���ȡһ���������͵�����ֵ
int ini_ReadInt(INIFILEHANDLE ini, const char *secName, const char *itemName, int Default);
// ��ini�ļ�д��һ���ַ������͵�����ֵ,���ini�ļ�û�и�����,�򴴽�
int ini_WriteString(INIFILEHANDLE ini, const char *secName, const char *itemName, const char *value);
// ��ini�ļ�д��һ���������͵�����ֵ,���ini�ļ�û�и�����,�򴴽�
int ini_WriteInt(INIFILEHANDLE ini, const char *secName, const char *itemName, int Value);
//additem
LPINIITEM ini_addItem(LPINISECTION iniSection, const char *name, const char *value);
//delitem
void ini_delItem(LPINISECTION iniSection, LPINIITEM item);
//add section
LPINISECTION ini_addSection(LPINIFILE iniFile, const char *name);
//del section
void ini_delSection(LPINIFILE iniFile, LPINISECTION sec);

// ��һ��ini�ļ�
//    iniFile: ini�ļ�·��
//    mode: �򿪷�ʽ, ��fopen�ĵڶ�������������ͬ
INIFILEHANDLE ini_open(const char *iniFile, const char *mode);


//C��ʽ��EFI��ʽת������
char* a2u(char*str);
//���ini��label����
int secNumbers(LPINIFILE ini);
//���label��item����
int itemNumbers(LPINISECTION sec);
#endif//_INI_H__
