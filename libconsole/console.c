/*
console.c
libconsole.h的实现
*/

#include <efi.h>
#include <efilib.h>

#include "libconsole.h"

//获取颜色
VOID conGetColor(OUT UINTN *fore,OUT UINTN *back){
    *fore = ST->ConOut->Mode->Attribute & 0x000000ff;
    *back =  (UINT8)(ST->ConOut->Mode->Attribute >> 4);
}
//设定颜色
VOID conSetColor(IN UINTN fore,IN UINTN back){
    ST->ConOut->SetAttribute (ST->ConOut, EFI_TEXT_ATTR(fore,back));
}
//清除屏幕
VOID conClearST(VOID){
    ST->ConOut->ClearScreen (ST->ConOut);
}
//设定当前光标位置
VOID conSetCursor(IN UINTN WIDTH,IN UINTN HEIGHT){
    ST->ConOut->SetCursorPosition(ST->ConOut,WIDTH,HEIGHT);
}
//设定光标是否显示
VOID conSetCursorDisplay(IN BOOLEAN Enable){
    ST->ConOut->EnableCursor(ST->ConOut,Enable);
}
//选定左右位置居中输出
VOID conStringOutMidLine(IN CHAR16 *string , IN UINTN Begin, IN UINTN End,IN UINTN row)
{
    UINTN strlen;
    UINTN printPos;
    CHAR16 *stringOut;
    strlen = StrLen(string);
    //check
    if(End<=Begin){
        return;
    }
    if(strlen>(End-Begin)){
        stringOut = (CHAR16*)AllocatePool(End-Begin);
        CopyMem((VOID*)stringOut,(VOID*)string,End-Begin);
    }
    else{
        stringOut = string;
    }
    printPos = Begin+(End-Begin-strlen)/2;
    PrintAt(printPos,row,L"%s",stringOut);
}

//填充一行
VOID conFillLine(IN CHAR16 c,IN UINTN Begin,IN UINTN End,IN UINTN row){
    CHAR16 *buffer;
    UINTN size = (End-Begin+1)+1;
    UINTN i;
    if(End<=Begin) return;
    buffer = (CHAR16*)AllocatePool(size*2);
    for(i=0;i<size-1;i++){
        buffer[i]=c;
    }
    buffer[size-1]=L'\0';
    PrintAt(Begin,row,L"%s",buffer);
    FreePool(buffer);
}

//填充一列
VOID conFillArrow(IN CHAR16 c,IN UINTN Begin,IN UINTN End,IN UINTN col){
    UINTN i;
    if(End<=Begin) return;
    for(i=Begin;i<=End;i++){
        PrintAt(col,i,L"%c",c);
    }
}