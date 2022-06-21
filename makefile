#-----------------------------------------------------------------------------
# These are the variables that are specific to your program
#-----------------------------------------------------------------------------
EXE = cppserver
#-----------------------------------------------------------------------------


#-----------------------------------------------------------------------------
# This is the name of the directory that holds the framework
#-----------------------------------------------------------------------------
CPPFW = cpp_framework
#-----------------------------------------------------------------------------


#-----------------------------------------------------------------------------
# Find out what kind of hardware we're compiling on
#-----------------------------------------------------------------------------
PLATFORM := $(shell uname)

#-----------------------------------------------------------------------------
# For x86, declare whether to emit 32-bit or 64-bit code
#-----------------------------------------------------------------------------
X86_TYPE = 64

#-----------------------------------------------------------------------------
# Declare where the object files get created
#-----------------------------------------------------------------------------
ARM_OBJ_DIR = obj_arm
X86_OBJ_DIR = obj_x86

#-----------------------------------------------------------------------------
# Tell 'make' what suffixes will appear in make rules
#-----------------------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .o .cpp

#-----------------------------------------------------------------------------
# Declare the compile-time flags that are common between all platforms
#-----------------------------------------------------------------------------
CXXFLAGS =	\
-O2 -g -Wall \
-Wno-write-strings \
-Wno-sign-compare \
-Wno-unused-result \
-Wno-strict-aliasing \
-std=c++11 \
-fcommon \
-I$(CPPUTILS_DIR) $(CPPFW) \
-DLINUX 

#-----------------------------------------------------------------------------
# Special compile time flags for ARM targets
#-----------------------------------------------------------------------------
ARMFLAGS = -march=armv7-a -mfpu=neon -mfloat-abi=hard -Wno-psabi

#-----------------------------------------------------------------------------
# If there was no goal on the command line, the default goal depends
# on what platform we're running on
#-----------------------------------------------------------------------------
ifeq ($(.DEFAULT_GOAL),)
    ifeq ($(PLATFORM), Linux)
        .DEFAULT_GOAL := x86
    endif
endif


#-----------------------------------------------------------------------------
# Define the name of the compiler and what "build all" means for our platform
#-----------------------------------------------------------------------------
ifeq ($(PLATFORM), Linux)
    ALL       = x86 arm
    ARM_CXX   = arm-none-linux-gnueabihf-g++
    ARM_STRIP = arm-none-linux-gnueabihf-strip
    X86_CXX   = $(CXX)
    X86_STRIP = strip
endif


#-----------------------------------------------------------------------------
# We're going to compile every .cpp file in this folder
#-----------------------------------------------------------------------------
SRC_FILES := $(shell ls *.cpp $(CPPFW)/*.cpp)

#-----------------------------------------------------------------------------
# Create the base-names of the object files
#-----------------------------------------------------------------------------
OBJ_FILES = $(SRC_FILES:.cpp=.o)

#-----------------------------------------------------------------------------
# We are going to keep x86 and ARM object files in separate sub-directories
#-----------------------------------------------------------------------------
X86_OBJS = $(addprefix $(X86_OBJ_DIR)/,$(OBJ_FILES))
ARM_OBJS = $(addprefix $(ARM_OBJ_DIR)/,$(OBJ_FILES))


#-----------------------------------------------------------------------------
# This rules tells how to compile an X86 .o object file from a .cpp source
#-----------------------------------------------------------------------------
$(X86_OBJ_DIR)/%.o : %.cpp
	$(X86_CXX) -m$(X86_TYPE) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

#-----------------------------------------------------------------------------
# This rules tells how to compile an ARM .o object file from a .cpp source
#-----------------------------------------------------------------------------
$(ARM_OBJ_DIR)/%.o : %.cpp
	$(ARM_CXX) $(CPPFLAGS) $(CXXFLAGS) $(ARMFLAGS) -c $< -o $@

#-----------------------------------------------------------------------------
# This rule builds the x86 executable from the object files
#-----------------------------------------------------------------------------
$(EXE).x86 : $(X86_OBJS)
	$(X86_CXX) -m$(X86_TYPE) -pthread -o $@ $(X86_OBJS)
	$(X86_STRIP) $(EXE).x86

#-----------------------------------------------------------------------------
# This rule builds the ARM executable from the object files
#-----------------------------------------------------------------------------
$(EXE).arm : $(ARM_OBJS)
	$(ARM_CXX)  -pthread $(ARMFLAGS) -o $@ $(ARM_OBJS) 
	$(ARM_STRIP) $(EXE).arm


#-----------------------------------------------------------------------------
# This target builds all executables supported by this platform
#-----------------------------------------------------------------------------
all:	$(ALL)

#-----------------------------------------------------------------------------
# This target builds just the ARM executable
#-----------------------------------------------------------------------------
arm:	mkdirs $(EXE).arm  

#-----------------------------------------------------------------------------
# This target builds just the x86 executable
#-----------------------------------------------------------------------------
x86:	mkdirs $(EXE).x86

#-----------------------------------------------------------------------------
# This target configures the object file directories
#-----------------------------------------------------------------------------
$(X86_OBJ_DIR):
	@mkdir $(X86_OBJ_DIR) $(X86_OBJ_DIR)/$(CPPFW)
$(ARM_OBJ_DIR):
	@mkdir $(ARM_OBJ_DIR) $(ARM_OBJ_DIR)/$(CPPFW)


#-----------------------------------------------------------------------------
# This target configured the object file directories
#-----------------------------------------------------------------------------
mkdirs:	$(X86_OBJ_DIR) $(ARM_OBJ_DIR)
	@chmod 777 $(X86_OBJ_DIR) $(X86_OBJ_DIR)/$(CPPFW)
	@chmod 777 $(ARM_OBJ_DIR) $(ARM_OBJ_DIR)/$(CPPFW)

#-----------------------------------------------------------------------------
# This target removes all files that are created at build time
#-----------------------------------------------------------------------------
clean:
	rm -rf Makefile.bak makefile.bak $(EXE).tgz $(EXE).x86 $(EXE).arm
	rm -rf $(X86_OBJ_DIR) $(ARM_OBJ_DIR)

#-----------------------------------------------------------------------------
# This target creates a compressed tarball of the source code
#-----------------------------------------------------------------------------
tarball:	clean
	rm -rf $(EXE).tgz
	tar --create --exclude-vcs -v -z -f $(EXE).tgz *

    
#-----------------------------------------------------------------------------
# This target appends/updates the dependencies list at the end of this file
#-----------------------------------------------------------------------------
depend:	x86
	makedepend -p$(X86_OBJ_DIR)/ -Y -I $(CPPFW) *.cpp 2>/dev/null


# DO NOT DELETE

obj_x86/cmd_server_base.o: cmd_server_base.h cpp_framework/cthread.h
obj_x86/cmd_server_base.o: cpp_framework/netsock.h
obj_x86/demo_server.o: demo_server.h cmd_server_base.h
obj_x86/demo_server.o: cpp_framework/cthread.h cpp_framework/netsock.h
obj_x86/main.o: demo_server.h cmd_server_base.h cpp_framework/cthread.h
obj_x86/main.o: cpp_framework/netsock.h cpp_framework/cmd_line.h
