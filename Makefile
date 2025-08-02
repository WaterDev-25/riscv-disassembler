CC = gcc
CFLAGS = -I./include -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -fsanitize=address
TARGET = rv32i-disassembler
SRCS := $(shell find src -name "*.c")
HEADERS := $(shell find include -name "*.h")
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean
