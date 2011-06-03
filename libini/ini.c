#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ini.h"
#include "queue.h"
//#include "_string_.h"

//获得label的item个数
int itemNumbers(LPINISECTION sec)
{
    LPINIITEM f,l,c;
    int r = 1;
    f = sec->first_item;
    l = sec->last_item;
    c=f;
    while(c!=l){
        c=c->next;
        r++;
    }
    return r;
}
//获得ini的label个数
int secNumbers(LPINIFILE ini)
{
    LPINISECTION f,l,c;
    int r = 1;
    f = ini->first_section;
    l = ini->last_section;
    c=f;
    while(c!=l){
        c=c->next;
        r++;
    }
    return r;
}

char* a2u(char*str){
	int i,size;
	char* s;
	size= strlen(str);
	s=(char*)malloc(size*2+2);
	for(i=0;i<size*2+2;i++){
		s[i]=0;
	}
	for(i=0;i<size;i++){
		s[i*2]=str[i];
	}
	s[i*2]='\0';
	return s;
}


char *strtrim(char *s)
{
    char *p = NULL,*right = s,*left = s;

    while(isspace(*left)) left++;
    p = left;
    while(*right != '\0') right++;
    for(right--;isspace(*right);right--);
    *(right+1) = '\0';
    strcpy(s,p);
    return s;
}

LPINIITEM ini_addItem(LPINISECTION iniSection, const char *name, const char *value)
{
	LPINIITEM item = (LPINIITEM)malloc(sizeof(INIITEM));
	item->name = (char *)malloc(strlen(name) + 1);
	strcpy(item->name, name);
	item->value = (char *)malloc(strlen(value) + 1);
	strcpy(item->value, value);
	add_node((void**)&(iniSection->first_item), (void**)&(iniSection->last_item), item);
	iniSection->itemCount++;
	return item;
}

void ini_delItem(LPINISECTION iniSection, LPINIITEM item)
{
	free(item->name);
	free(item->value);
	del_node((void**)&(iniSection->first_item), (void**)&(iniSection->last_item), item);
	free(item);
	iniSection->itemCount--;
}

LPINISECTION ini_addSection(LPINIFILE iniFile, const char *name)
{
	LPINISECTION sec = (LPINISECTION)malloc(sizeof(INISECTION));
	sec->name = (char *)malloc(strlen(name) + 1);
	strcpy(sec->name, name);
	sec->first_item = NULL;
	sec->last_item = NULL;
	sec->itemCount = 0;
	add_node((void**)&(iniFile->first_section), (void**)&(iniFile->last_section), sec);
	return sec;
}

void ini_delSection(LPINIFILE iniFile, LPINISECTION sec)
{
	LPINIITEM item = sec->first_item;
	free(sec->name);
	while(item)
	{
		LPINIITEM tmp = item->next;
		ini_delItem(sec, item);
		item = tmp;
	}
	del_node((void**)&(iniFile->first_section), (void**)&(iniFile->last_section), sec);
	free(sec);
}

static LPINIFILE ini_createIniStruct(const char *name)
{
	LPINIFILE iniFile = (LPINIFILE)malloc(sizeof(INIFILE));
	iniFile->name = (char *)malloc(strlen(name) + 1);
	strcpy(iniFile->name, name);
	iniFile->first_section = NULL;
	iniFile->last_section = NULL;
	iniFile->sectionCount = 0;
	iniFile->flag = 0;
	return iniFile;
}

static void ini_destryIniStruct(LPINIFILE iniFile)
{
	LPINISECTION sec = iniFile->first_section;
	free(iniFile->name);
	while(sec)
	{
		LPINISECTION tmp = sec->next;
		ini_delSection(iniFile, sec);
		sec = tmp;
	}
	free(iniFile);
}

static void ini_writeItem(FILE *fp, LPINIITEM item)
{
	fprintf(fp, "%s=%s\n", item->name, item->value);
}

static void ini_writeSection(FILE *fp, LPINISECTION sec)
{
	LPINIITEM item = sec->first_item;
	fprintf(fp, "[%s]\n", sec->name);
	while(item)
	{
		ini_writeItem(fp, item);
		item = item->next;
	}
	fprintf(fp, "\n");
}

static LPINIITEM ini_searchItem(LPINISECTION sec, const char *name)
{
	LPINIITEM item;
	if(sec == NULL)
		return NULL;
	item = sec->first_item;
	while(item)
	{
		if(strcmp(item->name, name) == 0)
			return item;
		item = item->next;
	}
	return NULL;
}

static LPINISECTION ini_searchSection(LPINIFILE iniFile, const char *name)
{
	LPINISECTION sec;
	if(iniFile == NULL)
		return NULL;
	sec = iniFile->first_section;
	while(sec)
	{
		if(strcmp(sec->name, name) == 0)
			return sec;
		sec = sec->next;
	}
	return NULL;
}

INIFILEHANDLE ini_open(const char *iniFile, const char *mode)
{
	char lineBuf[1024];
	LPINIFILE file;
	LPINISECTION curSection = NULL;
	FILE *fp;
	fp = fopen(iniFile, mode);
	if(fp == NULL)
		return NULL;
	file = ini_createIniStruct(iniFile);
	strcpy(file->mode, mode);
	while(fgets(lineBuf, 1023, fp))
	{
		char *p = strtrim(lineBuf);
		int len = strlen(p);
		if(strlen(p) <= 0)
			continue;
		else if(p[0] == '#')
			continue;
		else if((p[0] == '[') && (p[len - 1] == ']'))
		{
			p[len - 1] = '\0';
			p = strtrim(&p[1]);
			curSection = ini_addSection(file, p);
		}
		else
		{
			char *p2 = strchr(lineBuf, '=');
			if(p2 == NULL)
			{
				ini_destryIniStruct(file);
				file = NULL;
				break;
			}
			*p2++ = '\0';
			p = strtrim(p);
			p2 = strtrim(p2);
			ini_addItem(curSection, p, p2);
		}
	}
	fclose(fp);
	return (INIFILEHANDLE)file;
}

int ini_SaveTo(INIFILEHANDLE ini, FILE *fp)
{
	LPINIFILE iniFile = (LPINIFILE)ini;
	LPINISECTION sec = iniFile->first_section;
	if(fp == NULL)
		return -1;
	fprintf(fp, "# %s\n", iniFile->name);
	while(sec)
	{
		ini_writeSection(fp, sec);
		sec = sec->next;
	}
	return 0;
}

int ini_saveAs(INIFILEHANDLE ini, const char *saveFile)
{
	LPINIFILE iniFile = (LPINIFILE)ini;
	const char *filename = saveFile;
	FILE *fp;
	int ret;
	if(saveFile == NULL)
	{
		if(strchr(iniFile->mode, 'w') == 0)
			return -1;
		filename = iniFile->name;
	}
	fp = fopen(filename, "w+");
	if(fp == NULL)
		return -1;
	ret = ini_SaveTo(ini, fp);
	fclose(fp);
	return ret;
}

void ini_close(INIFILEHANDLE ini)
{
	LPINIFILE iniFile = (LPINIFILE)ini;
	if((iniFile->flag) && (strchr(iniFile->mode, 'w')))
		ini_saveAs(ini, NULL);
	ini_destryIniStruct(iniFile);
}

char *ini_ReadString(INIFILEHANDLE ini, const char *secName, const char *itemName, char *outBuf, const char *Default)
{
	LPINIITEM item = ini_searchItem(ini_searchSection((LPINIFILE)ini, secName), itemName);
	if(item == NULL)
	{
		if(outBuf == NULL)
			return (char *)Default;
		return strcpy(outBuf, Default);
	}
	if(outBuf == NULL)
		return item->value;
	return strcpy(outBuf, item->value);
}

int ini_ReadInt(INIFILEHANDLE ini, const char *secName, const char *itemName, int Default)
{
	LPINIITEM item = ini_searchItem(ini_searchSection((LPINIFILE)ini, secName), itemName);
	if(item == NULL)
		return Default;
	return atoi(item->value);
}

int ini_WriteString(INIFILEHANDLE ini, const char *secName, const char *itemName, const char *value)
{
	LPINIFILE iniFile = (LPINIFILE)ini;
	LPINISECTION sec;
	LPINIITEM item;
	iniFile->flag = 1;
	sec = ini_searchSection(iniFile, secName);
	if(sec == NULL)
	{
		sec = ini_addSection(iniFile, secName);
		item = ini_addItem(sec, itemName, value);
	}
	else
	{
		item = ini_searchItem(sec, itemName);
		if(item == NULL)
		{
			item = ini_addItem(sec, itemName, value);
		}
		else
		{
			free(item->value);
			item->value = (char *)malloc(strlen(value) + 1);
			strcpy(item->value, value);
		}
	}
	return 0;
}

int ini_WriteInt(INIFILEHANDLE ini, const char *secName, const char *itemName, int Value)
{
	char tmpString[16] = "";
	sprintf(tmpString, "%d", Value);
	return ini_WriteString(ini, secName, itemName, tmpString);
}
