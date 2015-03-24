
CPPC := g++
RM := rm
NAME := Underneath_BUILD
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)
CPP_FLAGS := -std=c++11
LIB_FLAGS := -lncurses

all: $(NAME)

$(NAME): $(OBJS)
	$(CPPC) $(OBJS) -o $(NAME) $(LIB_FLAGS)

$(OBJS): %.o: %.cpp
	$(CPPC) $(CPP_FLAGS) -c $< -o $@ $(LIB_FLAGS)

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

distclean: clean

