CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
CFLAGSTEST = -Wall -Wextra -Iinclude -I../c_unit/include

SRC = src/main.c src/bst.c
TESTSRC = test/bst_test.c src/bst.c ../c_unit/src/c_unit.c

.PHONY: all run clean test test_debug

all: main

main: $(SRC)
	$(CC) $(CFLAGS) -o main $(SRC)

run: main
	./main

test: $(TESTSRC)
	$(CC) $(CFLAGSTEST) -o test $(TESTSRC) $(CUNITSRC)
	./test

test_debug: $(TESTSRC)
	$(CC) -g $(CFLAGSTEST) -o test $(TESTSRC) $(CUNITSRC)
	gdb ./test

clean:
	rm -f main.exe test.exe
