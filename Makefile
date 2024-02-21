CC = clang
CFLAGS = -Wall -Wextra -g

SRCS = $(shell find src/ -type f -name '*.c')
OBJS = $(SRCS:.c=.o)
TARGET = mips_simulator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(TARGET)
	
