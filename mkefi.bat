
@echo off
echo --begin--
cd c:\EFI_Toolkit_2.0
call build.cmd nt32
cd apps\xmleditor
nmake -f test.mak clean
nmake -f test.mak all
echo --make--