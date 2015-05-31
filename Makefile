PROGRAM_NAME := $(shell basename $(CURDIR))

ifdef SystemRoot
  RM := del /Q
  MKDIR := mkdir
  ARCH := $(shell %PROCESSOR_ARCHITECTURE%)
  OSN := WINDOWS
else
  RM := rm
  MKDIR := mkdir
  OS := $(shell uname)
  ARCH := $(shell getconf LONG_BIT)

  ifeq ($(OS),Darwin)
    OSN := OSX
  else ifeq ($(OS),Linux)
    OSN := LINUX
  else
    OSN := $(shell $(OS) | tr A-Z a-z)
  endif
endif

CPPC := g++
PLATFORM := $(OSN)_$(ARCH)
NAME := $(PROGRAM_NAME)_BUILD_$(PLATFORM)
SRCS := $(wildcard src/*.cpp)
TMP := $(SRCS:.cpp=.o)
SRCSLASH := src/
OBJSLASH := obj/
OBJS := $(subst $(SRCSLASH),$(OBJSLASH),$(TMP))
CPP_FLAGS := -g -std=c++11
LIB_FLAGS := -lncurses

$(PROGRAM_NAME): $(NAME)

mingw32 : CPPC := i686-w64-mingw32-g++
mingw32 : NAME := $(NAME)_MINGW32.exe
mingw32 : CPP_FLAGS := -g -lstdc++ -std=c++11
mingw32 : LIB_FLAGS := -lpdcurses -static-libgcc -static-libstdc++ -lpthread 


all: clean $(NAME)

$(NAME): $(OBJS)
	$(CPPC) $(CPP_FLAGS) $(OBJS) -o $(NAME) $(LIB_FLAGS)

$(OBJSLASH)%.o: $(SRCSLASH)%.cpp
	@$(MKDIR) -p $(OBJSLASH)
	$(CPPC) $(CPP_FLAGS) -c $< -o $@

clean:
	@-$(RM) $(NAME)
	@-$(RM) $(OBJS)

mingw32: $(NAME)

