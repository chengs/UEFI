/*++

Copyright (c) 1998  Intel Corporation

Module Name:

    test.c
    
Abstract:

test


Revision History

--*/

#include "efi.h"
#include "efilib.h"

#include "menu/menu.h"
#include "libini/ini.h"

#include <stdio.h>
#include <stdlib.h>
#include <atk_libc.h>


	int
main ( int argc, char *argv[] )
{
	//变量声明
	INIFILEHANDLE ini;
	LPINIFILE ini1;
	Display dis;
	int r;
	
	if(argc<=1){
		perror("Input Error.\n");
		return 0;
	}
	
	Print(L"Init\n");
	Print(L"Read File\n");
	//读文件
	ini = ini_open(argv[1],"r");
	Print(L"Read Success\n");
	ini1 = (LPINIFILE)ini;
	dis.file = ini1;
	dis.curSecNum = 1;
	dis.curSec = ini1->first_section;
	dis.curItem = dis.curSec ->first_item;
	dis.curItemNum = 1;
	
	Print(L"display GUI\n");
	
	menuShow(&dis);
	r = menuWaitForKey(&dis);
	
	//保存文件
	Print(L"save file\n");
	if(r == 1){
        ini_saveAs(ini,argv[1]);
	}
	
	menuEnd();
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
