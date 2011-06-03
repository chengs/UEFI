/*
console.c
libconsole.h��ʵ��
*/

#include <efi.h>
#include <efilib.h>

#include "libconsole.h"

//��ȡ��ɫ
VOID conGetColor(OUT UINTN *fore,OUT UINTN *back){
    *fore = ST->ConOut->Mode->Attribute & 0x000000ff;
    *back =  (UINT8)(ST->ConOut->Mode->Attribute >> 4);
}
//�趨��ɫ
VOID conSetColor(IN UINTN fore,IN UINTN back){
    ST->ConOut->SetAttribute (ST->ConOut, EFI_TEXT_ATTR(fore,back));
}
//�����Ļ
VOID conClearST(VOID){
    ST->ConOut->ClearScreen (ST->ConOut);
}
//�趨��ǰ���λ��
VOID conSetCursor(IN UINTN WIDTH,IN UINTN HEIGHT){
    ST->ConOut->SetCursorPosition(ST->ConOut,WIDTH,HEIGHT);
}
//�趨����Ƿ���ʾ
VOID conSetCursorDisplay(IN BOOLEAN Enable){
    ST->ConOut->EnableCursor(ST->ConOut,Enable);
}
//ѡ������λ�þ������
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

//���һ��
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

//���һ��
VOID conFillArrow(IN CHAR16 c,IN UINTN Begin,IN UINTN End,IN UINTN col){
    UINTN i;
    if(End<=Begin) return;
    for(i=Begin;i<=End;i++){
        PrintAt(col,i,L"%c",c);
    }
}