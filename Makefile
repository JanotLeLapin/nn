CC := gcc
CFLAGS := -g -Wall -O3
LDFLAGS := -lm

.PHONY: test clean re

nn: main.o block.o instruction.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

nn-test: test/runner.o test/instruction.o block.o instruction.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lunity

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

test: nn-test
	./nn-test

clean:
	rm -rf nn *.o

re: clean nn
