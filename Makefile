CC = gcc
CFLAGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
OBJS=src/lexer/lexer.c src/lexer/token.c src/lib/string.c src/lib/vector.c


%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(OBJS)
	$(CC) -o main $(OBJS) main.c

test: $(OBJS)
	$(CC) -o test $(OBJS) test.c

run: all
	./main

fmt:
	clang-format -i main.c
	clang-format -i src/lexer/lexer.c
	clang-format -i src/lexer/token.c

clean:
	rm -f main test
	find . -name "*.o" | xargs rm -f
