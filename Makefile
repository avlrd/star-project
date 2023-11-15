CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
BIN_DIR = bin

all: $(BIN_DIR)/tar

$(BIN_DIR)/tar: $(SRC_DIR)/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/tar
