#
# Copyright (c) 1999, 2000
# Intel Corporation.
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
# 3. All advertising materials mentioning features or use of this software must
#    display the following acknowledgement:
# 
#    This product includes software developed by Intel Corporation and its
#    contributors.
# 
# 4. Neither the name of Intel Corporation or its contributors may be used to
#    endorse or promote products derived from this software without specific
#    prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY INTEL CORPORATION AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.  IN NO EVENT SHALL INTEL CORPORATION OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 

#
# Include sdk.env environment
#

!include $(SDK_INSTALL_DIR)\build\$(SDK_BUILD_ENV)\sdk.env

#
# Set the base output name and entry point
#

BASE_NAME         = xmleditor
#IMAGE_ENTRY_POINT = InitializeTestApplication

IMAGE_ENTRY_POINT = _LIBC_Start_Shellapp_A



#
# Globals needed by master.mak
#

TARGET_APP = $(BASE_NAME)
SOURCE_DIR = $(SDK_INSTALL_DIR)\apps\$(BASE_NAME)
BUILD_DIR  = $(SDK_BUILD_DIR)\apps\$(BASE_NAME)

#
# Include paths
#

!include $(SDK_INSTALL_DIR)\include\$(EFI_INC_DIR)\makefile.hdr
INC = -I $(SDK_INSTALL_DIR)\include\$(EFI_INC_DIR) \
      -I $(SDK_INSTALL_DIR)\include\$(EFI_INC_DIR)\$(PROCESSOR) $(INC)
INC = -I . \
      -I .\$(PROCESSOR) $(INC)
      
!include $(SDK_INSTALL_DIR)\include\bsd\makefile.hdr
INC = -I $(SDK_INSTALL_DIR)\include\bsd $(INC)


#
# Local libraries
#

LIBS = $(BUILD_DIR)\libconsole\libconsole.lib \
       $(BUILD_DIR)\libxml\libxml.lib \
       $(BUILD_DIR)\libeg\libeg.lib \
       $(BUILD_DIR)\libini\libini.lib \
       $(BUILD_DIR)\menu\menu.lib \
       $(BUILD_DIR)\libkeyboard\libkeyboard.lib \
	   $(LIBS)
	   

#
# Libraries
#

LIBS = $(LIBS) \
       $(SDK_BUILD_DIR)\lib\libc\libc.lib \
       $(SDK_BUILD_DIR)\lib\libm\libm.lib \
       $(SDK_BUILD_DIR)\lib\libsocket\libsocket.lib \
       $(SDK_BUILD_DIR)\lib\libdb\libdb.lib \
       $(SDK_BUILD_DIR)\lib\libz\libz.lib \
       $(SDK_BUILD_DIR)\lib\libefi\libefi.lib \
	   $(SDK_BUILD_DIR)\lib\libtty\libtty.lib \
	   $(SDK_BUILD_DIR)\lib\libsmbios\libsmbios.lib \
       $(SDK_BUILD_DIR)\lib\libefishell\libefishell.lib \
       
#
# Default target
#

all : dirs $(LIBS) $(OBJECTS) 


#
# Program object files
#

OBJECTS = $(OBJECTS) \
    $(BUILD_DIR)\test.obj 
    
#
# Local libraries dependencies
#
  
$(BUILD_DIR)\libconsole\libconsole.lib : 
	cd libconsole
	nmake -f libconsole.mak all
	cd $(SOURCE_DIR)  
	  
$(BUILD_DIR)\libeg\libeg.lib : 
	cd libeg
	nmake -f libeg.mak all
	cd $(SOURCE_DIR)  

$(BUILD_DIR)\libxml\libxml.lib : 
	cd libxml
	nmake -f libxml.mak all
	cd $(SOURCE_DIR)  
	
$(BUILD_DIR)\menu\menu.lib : 
	cd menu
	nmake -f menu.mak all
	cd $(SOURCE_DIR)  
	  
$(BUILD_DIR)\libini\libini.lib : 
	cd libini
	nmake -f libini.mak all
	cd $(SOURCE_DIR)  
	

$(BUILD_DIR)\libkeyboard\libkeyboard.lib : 
	cd libkeyboard
	nmake -f libkeyboard.mak all
	cd $(SOURCE_DIR)  


#
# Source file dependencies
#

$(BUILD_DIR)\test.obj : $(*B).c $(INC_DEPS)

#
# Handoff to master.mak
#

!include $(SDK_INSTALL_DIR)\build\master.mak
