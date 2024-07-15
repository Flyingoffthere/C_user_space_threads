CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude -masm=intel 

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
ASM_DIR = asm

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(BIN_DIR)/main
ASM_OUTPUT = $(ASM_DIR)/main.s

ifdef OUTPUT_ASM
all: $(ASM_OUTPUT) $(TARGET)
else
all: $(TARGET)
endif

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ASM_OUTPUT): $(TARGET)
	@mkdir -p $(ASM_DIR)
	objdump -d -M intel $(TARGET) > $(ASM_OUTPUT)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(ASM_DIR)

.PHONY: all clean
