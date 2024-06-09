# Install
BIN = CNotes

# Flags
CFLAGS += -std=c11 -Wall -Wextra -pedantic -Wno-unused-function -O2

SRC = src/app.c
OBJ = $(SRC:.c=.o)

$(BIN):
	@mkdir -p build
	rm -f build/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -D_POSIX_C_SOURCE=200809L -o build/$(BIN) -lX11 -lm
