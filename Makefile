CFLAGS   = -std=c99 -Wall -Wextra -pedantic -g
CPPFLAGS += -Iinc

BLD 	= build
SRC_C   = $(wildcard src/*.c)
OBJ     = $(SRC_C:%.c=$(BLD)/%.o)

TRG = chat

all: $(TRG)

$(TRG): $(OBJ)
	gcc -o $@ $^

$(BLD)/%.o: %.c
	@mkdir -p $(@D)
	gcc $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(BLD) $(TRG)

.PHONY: clean all
