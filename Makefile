# compiler
CC = gcc

# directories
BIN_DIR   = bin
BUILD_DIR = build

# compilation flags
CFLAGS           = -std=c89 -Wall -Wextra -Wpedantic -Wcast-qual -Wconversion
OPTIMIZE_RELEASE = -O3

# object file
SUDOKU_OBJ = $(BUILD_DIR)/sudoku.o

# default target
all: $(BIN_DIR)/sudoku

# executable target
$(BIN_DIR)/sudoku: $(SUDOKU_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OPTIMIZE_RELEASE) -o $@ $^

# object target
$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(OPTIMIZE_RELEASE) -o $@ $<

# clean target
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)

# .PHONY target
.PHONY: all clean
