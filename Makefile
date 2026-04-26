CFLAGS   = -std=c99 -Wall -Wextra -pedantic -g
CPPFLAGS += -Iinc

SRC_C   = $(wildcard src/*.c)
OBJ     = $(SRC_C:.c=.o)

TRG = chat
OBJ_MAIN = chat.o

all: $(TRG)

$(TRG): $(OBJ_MAIN) $(OBJ)
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TRG) $(OBJ_MAIN)

.PHONY: clean all
