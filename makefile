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

CPPC ?= g++
PLATFORM ?= $(OSN)_$(ARCH)
NAME := Underneath_BUILD_$(PLATFORM)
SRCS := $(wildcard src/*.cpp)
TMP := $(SRCS:.cpp=.o)
SRCSLASH := src/
OBJSLASH := obj/
OBJS := $(subst $(SRCSLASH),$(OBJSLASH),$(TMP))
CPP_FLAGS := -g -std=c++11
LIB_FLAGS := -lncurses


all: $(NAME)
#	echo $(OBJS)

$(NAME): $(OBJS)
	$(CPPC) $(CPP_FLAGS) $(OBJS) -o $(NAME) $(LIB_FLAGS)

$(OBJSLASH)%.o: $(SRCSLASH)%.h

$(OBJSLASH)%.o: $(SRCSLASH)%.cpp
	@$(MKDIR) -p $(OBJSLASH)
	$(CPPC) $(CPP_FLAGS) -c $< -o $@

clean:
	@-$(RM) $(NAME)
	@-$(RM) $(OBJS)
