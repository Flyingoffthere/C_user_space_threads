CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude -masm=intel
LDFLAGS = -lm

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib
DEBUG_DIR = debug

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
LIB_SRC = $(wildcard $(SRC_DIR)/custom_ucontext.c)
LIB_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SRC))
LIB_NAME = libcustom_ucontext.a
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)
DBG = dump.txt

TARGET = $(BIN_DIR)/main

all: $(TARGET)

$(TARGET): $(LIB_TARGET) $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) -L$(LIB_DIR) -lcustom_ucontext

$(LIB_TARGET): $(LIB_OBJ)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(TARGET)
	@mkdir -p $(DEBUG_DIR)
	touch $(DEBUG_DIR)/$(DBG) 
	objdump -d -S $(TARGET) > $(DEBUG_DIR)/$(DBG) 

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR) $(DEBUG_DIR)

.PHONY: all clean debug