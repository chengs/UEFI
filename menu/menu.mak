#By chengs
#libxml
#origin version xmlparse
#
# Include sdk.env environment
#

!include $(SDK_INSTALL_DIR)\build\$(SDK_BUILD_ENV)\sdk.env

#
# Set the base output name and type for this makefile
#

BASE_NAME  = menu

#
# Globals needed by master.mak
#

TARGET_LIB = $(BASE_NAME)
SOURCE_DIR = $(SDK_INSTALL_DIR)\apps\xmleditor\$(BASE_NAME)
BUILD_DIR  = $(SDK_BUILD_DIR)\apps\xmleditor\$(BASE_NAME)

#
# Include paths
#

!include $(SDK_INSTALL_DIR)\include\$(EFI_INC_DIR)\makefile.hdr
INC = -I $(SDK_INSTALL_DIR)\include\$(EFI_INC_DIR) \
      -I $(SDK_INSTALL_DIR)\include\$(EFI_INC_DIR)\$(PROCESSOR)

#!include .\makefile.hdr
INC = -I . \
      -I .\$(PROCESSOR) $(INC)
      
!include $(SDK_INSTALL_DIR)\include\bsd\makefile.hdr
INC = -I $(SDK_INSTALL_DIR)\include\bsd $(INC)

#
# Libraries
#

LIBS = $(LIBS) \
       $(SDK_BUILD_DIR)\lib\libefi\libefi.lib \
       $(SDK_BUILD_DIR)\lib\libefishell\libefishell.lib \
       

all : dirs $(LIBS) $(OBJECTS)

#
#  Program object files
#
OBJECTS = $(OBJECTS) \
    $(BUILD_DIR)\menu.obj

#
# Source file dependencies
#

$(BUILD_DIR)\menu.obj : $(*B).c $(INC_DEPS)

#
# Handoff to master.mak
#

!include $(SDK_INSTALL_DIR)\build\master.mak
