CC = gcc
CFLAGS = -g -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -O0
OBJS=src/lexer/lexer.c src/lexer/token.c\
     src/lib/list.c src/lib/string.c src/lib/vector.c\
     src/parser/ast.c src/parser/parser.c


%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: $(OBJS)
	$(CC) -o main $(OBJS) main.c

test: $(OBJS)
	$(CC) -o test $(OBJS) test.c

memtest: all
	valgrind --leak-check=full --log-file="valgrind.log" ./main

run: all
	./main

fmt:
	clang-format -i main.c
	clang-format -i $(OBJS)

clean:
	rm -f main test
	find . -name "*.o" | xargs rm -f
