CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
CFLAGS   = -std=c99 -Wall -Wextra -pedantic -g
CPPFLAGS += -I../../inc

SRC_CPP = main.cpp $(wildcard ../../src/chat/*.cpp)
SRC_C = $(wildcard ../../src/chat/*.c)
OBJ = $(addsuffix .o, $(basename $(SRC_CPP) $(SRC_C)))
TRG = chat

all: $(TRG)

$(TRG): $(OBJ)
	g++ -o $(TRG) $(OBJ)

%.o: %.cpp
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(TRG) $(OBJ)

.PHONY: clean all
