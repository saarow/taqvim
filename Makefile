CC = gcc
CFLAGS = -Wall -Wextra -o 
SOURCES_DIR = ./src/main.c ./src/utils.c
BIN_DIR = ./bin
EXECUTABLE = $(BIN_DIR)/taqvim

build: clear
	@mkdir $(BIN_DIR) 
	@$(CC) $(CFLAGS) $(EXECUTABLE) $(SOURCES_DIR) -lcurl

run: build
	@$(EXECUTABLE)

clear:
	@rm -rf $(BIN_DIR)
