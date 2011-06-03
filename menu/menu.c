/*
menu.c
*/

#include <efi.h>
#include <efilib.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../libconsole/libconsole.h"
#include "menu.h"
#include "../libkeyboard/key.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_s
 *  Description:  修正版输入函数
 * =====================================================================================
 */
char *gets_s(char *s,int n)
{
  register int c;
  register char *cs;
  cs=s;
  while(--n>0 && (c=getc(stdin))!=EOF && c!='\n')
  *cs++=c;
  *cs='\0';
  return (c==EOF && cs==s) ||(c=='\n' && cs==s)? NULL : s ;
}
VOID menuCleanHalf()
{
	UINTN line = WIN_MAIN_BEGIN+1;
	UINTN i;
	for(i=1;i<=WIN_MAIN_HEIGHT;i++){
		conFillLine(L' ',0,WIN_WIDTH,line+i);
	}
}
VOID menuReqOut(char *req);
//获得输入

int menuGetBool(char *req)
{
	char *r;
	char *buff = (char*)malloc(300);
	do
	{
		menuReqOut(req);
		r = gets_s(buff,255);
		conFillLine(L' ',0,WIN_WIDTH,WIN_MAIN_BEGIN+2+WIN_MAIN_HEIGHT);
	}
	while( r == NULL || 
	 	(strcmp(r,"yes") != 0 && strcmp(r,"no") != 0)
	     );
	if ( strcmp(r,"yes") == 0 ){
		free(buff);
		return 1;
	}
	else if( strcmp(r,"no") == 0 ){
		free(buff);
		return 0;
	}
	else	{
		free(buff);
		return -1;
	}
}

char* menuGetString(char *req)
{
	char *r;
	char *buff = (char*)malloc(300);
	//显示要求
	do
	{
		menuReqOut(req);
		r = gets_s(buff,255);
		conFillLine(L' ',0,WIN_WIDTH,WIN_MAIN_BEGIN+2+WIN_MAIN_HEIGHT);
	}
	while(r==NULL);
	return r;
}
//显示要求
VOID menuReqOut(char *req){
    UINTN fore,back;
    char *cache;
    conGetColor(&fore,&back);
    conSetColor(EFI_YELLOW,fore);
    //输出之前先填充颜色
    conFillLine(L' ',0,WIN_WIDTH,WIN_MAIN_BEGIN+2+WIN_MAIN_HEIGHT);
    conSetCursor(0,WIN_MAIN_BEGIN+2+WIN_MAIN_HEIGHT);
    Print(L"%s",a2u(req));
    //回复
    conSetColor(fore,back);

}
//显示
VOID menuShow(Display *d)
{
	//清屏幕
	conClearST();
	conSetCursorDisplay(TRUE);
	
	//外框
	menuHeader();
	menuMainFrame();
	
	//主体
	menuLabelList(d->file,d->curSecNum);
	menuAttrList(d);

	//结束
	menuFooter();

	//welcome message
	menuReqOut("Welcome!");
}

VOID disItemU(Display *d){
	int itemN = itemNumbers(d->curSec);
	d->curItemNum--;
	if(d->curItemNum < 1){
		d->curItemNum = itemN;
		d->curItem = d->curSec->last_item;
	}
	else
	{
		d->curItem = d->curItem->prev;
	}
	menuAttrList(d);
}

VOID disItemD(Display *d){
	int itemN = itemNumbers(d->curSec);
	d->curItemNum++;
	if(d->curItemNum > itemN){
		d->curItemNum = 1;
		d->curItem = d->curSec->first_item;
	}
	else
	{
		d->curItem = d->curItem->next;
	}
	menuAttrList(d);
}

VOID disSecL(Display *d){
	int secC = secNumbers(d->file);
	d->curSecNum--;
	if(d->curSecNum < 1){
		d->curSecNum = secC;
		d->curSec = d->file->last_section;
	}
	else
	{
		d->curSec = d->curSec->prev;
	}
	d->curItem = d->curSec->first_item;
	d->curItemNum = 1;	
	menuShow(d);
}

VOID disSecR(Display *d){
	int secC = secNumbers(d->file);
	d->curSecNum++;
	if(d->curSecNum > secC){
		d->curSecNum = 1;
		d->curSec = d->file->first_section;
	}
	else
	{
		d->curSec = d->curSec->next;
	}
	d->curItem = d->curSec->first_item;
	d->curItemNum = 1;	
	menuShow(d);
}

//绘制头部
VOID menuHeader(){
    UINTN fore,back;
    conGetColor(&fore,&back);
    conSetColor(EFI_YELLOW,fore);
    conFillLine(L' ',0,WIN_WIDTH,0);
    conFillLine(L' ',0,WIN_WIDTH,1);
    conStringOutMidLine(WELCOME_TITLE,0,WIN_WIDTH,0);
    conStringOutMidLine(WELCOME_CODER,0,WIN_WIDTH,1);
    conSetColor(fore,back);
}
//绘制主界面框架:
VOID menuMainFrame(){
    UINTN fore,back;
    conGetColor(&fore,&back);
    conSetColor(fore,EFI_BLUE);
    //from line 3
    conFillLine(L' ',0,WIN_WIDTH,WIN_MAIN_BEGIN);
    conFillArrow(L' ',WIN_MAIN_BEGIN,WIN_MAIN_BEGIN+1+WIN_MAIN_HEIGHT,0);
    conFillArrow(L' ',WIN_MAIN_BEGIN,WIN_MAIN_BEGIN+1+WIN_MAIN_HEIGHT,WIN_WIDTH);
    conFillArrow(L' ',WIN_MAIN_BEGIN,WIN_MAIN_BEGIN+1+WIN_MAIN_HEIGHT,WIN_WIDTH);
    conFillArrow(L' ',WIN_MAIN_BEGIN,WIN_MAIN_BEGIN+1+WIN_MAIN_HEIGHT,WIN_WIDTH/2);
    conFillLine(L' ',0,WIN_WIDTH,WIN_MAIN_BEGIN+1+WIN_MAIN_HEIGHT);
    conSetColor(EFI_YELLOW,EFI_BLUE);
    conStringOutMidLine(L"Key",0,WIN_WIDTH/2-1,WIN_MAIN_BEGIN);
    conStringOutMidLine(L"value",WIN_WIDTH/2+1,WIN_WIDTH,WIN_MAIN_BEGIN);
    conSetColor(fore,back);
}

//绘制section 列表g
VOID menuSectionList(LPINIFILE ini)
{
	char *cache;
	UINTN fore,back;
	UINTN flag=0;
	UINTN line=WIN_MAIN_BEGIN+2;
	LPINISECTION f = ini->first_section;
	LPINISECTION l = ini->last_section;
	LPINISECTION p = f;
	if(f==NULL) return;
	//改变文字颜色
	conGetColor(&fore,&back);
	conSetColor(EFI_YELLOW,back);
	while(flag==0)
	{
		cache=a2u(p->name);
		PrintAt(4,line,L"[%s]",cache);
		free(cache);
		if ( p==l ) {
			break;
		} else {
			p=p->next;
			line++;
		}
	}
	//恢复文字颜色
	conSetColor(fore,back);


}

//绘制attr 列表
VOID menuAttrList(Display *d){
	LPINISECTION section = d->curSec;
	char* cache1,*cache2;
	UINTN fore,back;
	UINTN flag=0;
	UINTN line=WIN_MAIN_BEGIN+2;
	UINTN right=WIN_WIDTH/2+1+3;
	UINTN left = 4;
	LPINIITEM f,p,l;

	menuCleanHalf();
	menuMainFrame();
	menuFooter();
	conGetColor(&fore,&back);
	conSetColor(EFI_YELLOW,back);
	f=section->first_item;
	if(f==NULL) return;
	l=section->last_item;
	p=f;
	if(f==NULL) return;
	while(flag==0)
	{
		if( p == d->curItem ){
			conSetColor(EFI_YELLOW,EFI_GREEN);
		}
		else {
			conSetColor(EFI_YELLOW,back);
		}
		cache1 = a2u(p->value);
		cache2 = a2u(p->name);
		PrintAt(left,line,L"%s",cache2);
		PrintAt(right,line,L"%s",cache1);
		free(cache1);free(cache2);
		if ( p==l ) {
			break;
		} else {
			p=p->next;
			line++;
		}
	
	}
	conSetColor(fore,back);

	
}

//结束动作
VOID menuEnd()
{
	//程序结束时完成清理动作
	conClearST();	
}

//绘制 label
VOID menuLabelList(LPINIFILE ini, int checked)
{
	UINTN fore,back;
	char *cache;
	LPINISECTION f,l,cur;
	int count=1;
	conGetColor(&fore,&back);
	f = ini->first_section;
	l = ini->last_section;
	//检查checked 变量有效性
	if(checked<=0 || checked > secNumbers(ini) ){
		checked = 1;
	}
	//设定光标
	conSetCursor(0,WIN_MAIN_BEGIN-1);
	for(cur=f;;cur=cur->next,count++){
		if ( count != checked ) {
			conSetColor(EFI_YELLOW,EFI_BLUE);
		} else {
			conSetColor(EFI_YELLOW,EFI_GREEN);
		}
		cache = a2u(cur->name);
		Print(L" [%s] ",cache);
		free(cache);
		if(cur == l) break;
	}
	//回复颜色
	conSetColor(fore,back);
}

//等待按键
int menuWaitForKey(Display *d){
	char *input,*inputn,*inputv;
	LPINIFILE ini = d->file;
	LPINIITEM item;
	LPINISECTION section;
	int checked;
	int sectionCount = secNumbers(ini);
	UINT16 key=0;
	checked=1;
	while(key != KEY_ESC){
		key = keyGetInput();
		//Print(L"%d",sectionCount);
		if( key == KEY_ESC ){
			return menuGetBool("Do you want to save?(yes or no)");
		}
		else if (key == KEY_LEFT){
			disSecL(d);
		}
		else if (key == KEY_RIGHT){
			disSecR(d);
		}
		else if (key == KEY_UP){
			disItemU(d);
		}
		else if (key == KEY_DOWN){
			disItemD(d);
		}
		else if (key == KEY_ENTER){
			input = menuGetString("Please Input a string:");
			d->curItem->value = input;
			menuAttrList(d);
		}
		else if (key == KEY_F1){
			//add item
			inputn = menuGetString("Input item name:");
			inputv = menuGetString("Input item value:");
			ini_addItem(d->curSec,inputn,inputv);
			d->curItem = d->curSec->first_item;
			d->curItemNum = 1;
			/*
			item = (LPINIITEM)malloc(sizeof(INIITEM));
			item->name = inputn;
			item->value = inputv;
			item->next = d->curItem->next;
			item->prev = d->curItem;
			d->curItem->next = item;
			*/
			menuAttrList(d);
		}
		else if (key == KEY_F2){
			if(d->curSec->first_item == d->curSec->last_item){
				menuReqOut("There must be one item at least.");
				continue;
			}
			//delete item
			ini_delItem(d->curSec,d->curItem);
			d->curItem = d->curSec->first_item;
			d->curItemNum = 1;
			menuAttrList(d);
		}
		else if (key == KEY_F3){
			//add section
			input = menuGetString("Section Name:");
			inputn = menuGetString("First Item Name:");
			inputv = menuGetString("First Item Value:");
			d->curSec = ini_addSection(d->file,input);
			ini_addItem(d->curSec,inputn,inputv);
			d->curSec = d->file->first_section;
			d->curSecNum = 1;
			d->curItem = d->curSec->first_item;
			d->curItemNum = 1;
			menuShow(d);
		}
		else if (key == KEY_F4){
			//delete section
			if(d->file->first_section == d->file->last_section){
				menuReqOut("There must be one section at least");
				continue;
			}
			ini_delSection(d->file,d->curSec);
			d->curSec = d->file->first_section;
			d->curSecNum = 1;
			d->curItem = d->curSec->first_item;
			d->curItemNum = 1;
			menuShow(d);
		}
		else {
			continue;
			//return;
		}
	}
	//不会运行到这里
	return 0;

}
#define QUICK_KEY_STRING1 L"Esc   Quit       F1 Add item       F3  Add Section"
#define QUICK_KEY_STRING2 L"Enter Set Value  F2 Delete item    F4  Delete Section"
#define QUICK_KEY_STRING3 L" "
//绘制快捷键说明
VOID menuFooter(){
	UINTN fore,back;
	UINTN line = WIN_MAIN_BEGIN+3+WIN_MAIN_HEIGHT;
	conGetColor(&fore,&back);
	conSetColor(EFI_YELLOW,EFI_BLUE);
	//填充颜色
	conFillLine(L' ',0,WIN_WIDTH,line);
	conFillLine(L' ',0,WIN_WIDTH,line+1);
	//conFillLine(L' ',0,WIN_WIDTH,line+2);
	//输出
	PrintAt(0,line,QUICK_KEY_STRING1);
	PrintAt(0,line+1,QUICK_KEY_STRING2);
	//PrintAt(0,line+2,QUICK_KEY_STRING3);
	//恢复颜色
	conSetColor(fore,back);
}


