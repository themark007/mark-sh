CC=gcc
CFLAGS=-Iinclude -Wall -Wextra
SRC=src/main.c src/builtins.c src/parser.c src/utils.c src/history.c src/readline.c
OBJ=$(SRC:.c=.o)
TARGET=marksh

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
