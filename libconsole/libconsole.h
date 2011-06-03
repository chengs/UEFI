/*libconsole.h
    重新组合console的各个接口
*/

#ifndef __LIBCONSOLE_LIBCONSOLE_H__
#define __LIBCONSOLE_LIBCONSOLE_H__
/*
//颜色代码
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

//获取颜色
VOID conGetColor(OUT UINTN *fore,OUT UINTN *back);
//设定颜色
VOID conSetColor(IN UINTN fore,IN UINTN back);
//清除屏幕
VOID conClearST(VOID);
//获得当前光标位置
//VOID conGetCursor(OUT UINTN *WIDTH,OUT UINTN *HEIGHT);
//设定当前光标位置
VOID conSetCursor(IN UINTN WIDTH,IN UINTN HEIGHT);
//设定光标是否显示
VOID conSetCursorDisplay(IN BOOLEAN Enable);
//填充一行
VOID conFillLine(IN CHAR16 c,IN UINTN Begin,IN UINTN End,IN UINTN row);
//填充一列
VOID conFillArrow(IN CHAR16 c,IN UINTN Begin,IN UINTN End,IN UINTN col);
//选定左右位置居中输出
VOID conStringOutMidLine(IN CHAR16 *string , IN UINTN Begin, IN UINTN End,IN UINTN row);

#endif