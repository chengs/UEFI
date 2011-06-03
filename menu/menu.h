/*
menu.h
���ƴ���
*/
/*
������Ҫ����
*/
//����INI ��
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
//��ʾ
VOID menuShow(Display *d);
//����ͷ��
VOID menuHeader();
//������������
VOID menuMainFrame();
//���ƿ�ݼ�˵��
VOID menuFooter();
//Ŀ¼ģʽ
VOID menuQuickKeyList();
//�����޸�ģʽ
VOID menuQuickKeyAttr();
//����ֵ�趨����
VOID menuAttrSetWindow();
//����section �б�
VOID menuSectionList(LPINIFILE ini);
//����attr �б�
VOID menuAttrList(Display *);
//��������
VOID menuEnd();
//���� label
VOID menuLabelList(LPINIFILE ini, int checked);
//�ȴ�����
int menuWaitForKey(Display *d);
