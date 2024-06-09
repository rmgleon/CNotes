# Install
BIN = zahnrad

# Flags
CFLAGS += -std=c11 -Wall -Wextra -pedantic -Wno-unused-function -O2

SRC = app.c
OBJ = $(SRC:.c=.o)

$(BIN):
	@mkdir -p bin
	rm -f bin/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o bin/$(BIN) -lX11 -lm
