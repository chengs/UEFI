/*libconsole.h
    �������console�ĸ����ӿ�
*/

#ifndef __LIBCONSOLE_LIBCONSOLE_H__
#define __LIBCONSOLE_LIBCONSOLE_H__
/*
//��ɫ����
UINTN EfiColor[] = {
    EFI_BLACK,  
    EFI_BLUE,    
    EFI_GREEN,   
    EFI_CYAN,    
    EFI_RED,     
    EFI_MAGENTA, 
    EFI_BROWN,   
    EFI_LIGHTGRAY,                 
    EFI_DARKGRAY,        
    EFI_LIGHTBLUE,       
    EFI_LIGHTGREEN,      
    EFI_LIGHTCYAN,       
    EFI_LIGHTRED,        
    EFI_LIGHTMAGENTA,    
    EFI_YELLOW,          
    EFI_WHITE           
};*/
/*
#define EFI_TEXT_ATTR(foreground,background) \
  ((foreground) | ((background) << 4))
*/

//��ȡ��ɫ
VOID conGetColor(OUT UINTN *fore,OUT UINTN *back);
//�趨��ɫ
VOID conSetColor(IN UINTN fore,IN UINTN back);
//�����Ļ
VOID conClearST(VOID);
//��õ�ǰ���λ��
//VOID conGetCursor(OUT UINTN *WIDTH,OUT UINTN *HEIGHT);
//�趨��ǰ���λ��
VOID conSetCursor(IN UINTN WIDTH,IN UINTN HEIGHT);
//�趨����Ƿ���ʾ
VOID conSetCursorDisplay(IN BOOLEAN Enable);
//���һ��
VOID conFillLine(IN CHAR16 c,IN UINTN Begin,IN UINTN End,IN UINTN row);
//���һ��
VOID conFillArrow(IN CHAR16 c,IN UINTN Begin,IN UINTN End,IN UINTN col);
//ѡ������λ�þ������
VOID conStringOutMidLine(IN CHAR16 *string , IN UINTN Begin, IN UINTN End,IN UINTN row);

#endif