CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude -masm=intel
LDFLAGS = -lm

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DEBUG_DIR = debug

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DBG = dump.txt

TARGET = $(BIN_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(TARGET)
	@mkdir -p $(DEBUG_DIR)
	touch $(DEBUG_DIR)/$(DBG) 
	objdump -d -S $< > $(DEBUG_DIR)/$(DBG) 

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean debug