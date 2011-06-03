/*
 * =====================================================================================
 *
 *       Filename:  key.c
 *
 *    Description:  key.h ��ʵ��
 *
 *        Version:  1.0
 *        Created:  2011/5/10 12:54:57
 *
 * =====================================================================================
 */


#include	"efi.h"
#include	"efilib.h"

#include	"key.h"

//��ü�������ɨ����
UINT16 keyGetInput(){
	EFI_INPUT_KEY Key;
	EFI_STATUS Status;
	WaitForSingleEvent (ST->ConIn->WaitForKey, 0);
	Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key);
	if(Key.UnicodeChar == 0xD) //enter
		return KEY_ENTER;
	else
		return Key.ScanCode;
}
