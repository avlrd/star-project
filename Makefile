CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

all: $(BIN_DIR)/tar

$(BIN_DIR)/tar: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

doc:
	doxygen Doxyfile

clean:
	rm -f $(BIN_DIR)/*