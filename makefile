
CPPC := g++
RM := rm
NAME := Underneath_BUILD_LINUX64
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
	$(CPPC) $(CPP_FLAGS) -c $< -o $@ $(LIB_FLAGS)

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

distclean: clean
