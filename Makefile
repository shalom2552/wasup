CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
CFLAGS   = -std=c99 -Wall -Wextra -pedantic -g
CPPFLAGS += -Iinc

SRC_CPP = $(wildcard src/*.cpp)
SRC_C   = $(wildcard src/*.c)
OBJ     = $(SRC_C:.c=.o) $(SRC_CPP:.cpp=.o)

TRG = chat
OBJ_MAIN = chat.o

all: $(TRG)

$(TRG): $(OBJ_MAIN) $(OBJ)
	g++ -o $@ $^

%.o: %.cpp
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TRG) $(OBJ_MAIN)

.PHONY: clean all
