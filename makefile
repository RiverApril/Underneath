
CPPC := g++
NAME := Underneath_BUILD
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)
CPP_FLAGS := -std=c++11 -Wall
LIB_FLAGS := -lncurses

all: $(NAME)

$(NAME):# $(OBJS)
	$(CPPC) $(CPP_FLAGS) -o $(NAME) $(SRCS) $(LIB_FLAGS)

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

distclean: clean

