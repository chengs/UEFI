/*
menu.h
绘制窗口
*/
/*
声明主要参数
*/
//导入INI 库
#include "../libini/ini.h"

#define WIN_WIDTH 70
#define WIN_MAIN_HEIGHT 15
#define WIN_MAIN_BEGIN 4
#define WELCOME_TITLE L"INI File Editor"
#define WELCOME_CODER L"Chenguangshuo"

typedef struct {
	LPINIFILE file;
	LPINISECTION curSec;
	int curSecNum;
	LPINIITEM curItem;
	int curItemNum;
} Display;
//显示
VOID menuShow(Display *d);
//绘制头部
VOID menuHeader();
//绘制主界面框架
VOID menuMainFrame();
//绘制快捷键说明
VOID menuFooter();
//目录模式
VOID menuQuickKeyList();
//属性修改模式
VOID menuQuickKeyAttr();
//绘制值设定窗口
VOID menuAttrSetWindow();
//绘制section 列表
VOID menuSectionList(LPINIFILE ini);
//绘制attr 列表
VOID menuAttrList(Display *);
//结束动作
VOID menuEnd();
//绘制 label
VOID menuLabelList(LPINIFILE ini, int checked);
//等待按键
int menuWaitForKey(Display *d);
