CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
CFLAGS   = -std=c99 -Wall -Wextra -pedantic -g
CPPFLAGS += -I../../inc

SRC_CPP = $(wildcard ../../src/chat/*.cpp)
SRC_C = $(wildcard ../../src/chat/*.c)
OBJ = $(SRC_C:.c=.o) $(SRC_CPP:.cpp=.o)

TRG_SERVER = run_server
TRG_CLIENT = run_client
OBJ_SERVER = run_server.o
OBJ_CLIENT = run_client.o

all: $(TRG_SERVER) $(TRG_CLIENT)

$(TRG_SERVER): $(TRG_SERVER).o $(OBJ)
	g++ -o $(TRG_SERVER) $^

$(TRG_CLIENT): $(TRG_CLIENT).o $(OBJ)
	g++ -o $(TRG_CLIENT) $^

%.o: %.cpp
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TRG_SERVER) $(TRG_CLIENT) $(OBJ_SERVER) $(OBJ_CLIENT)

.PHONY: clean all
