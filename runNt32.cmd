set DIR=%cd%
nmake run
cd c:\FW\Edk\Sample\Platform\Nt32\Build
set EDK_SOURCE=c:\FW\Edk
@echo off
call System.cmd

nmake run
cd %DIR%