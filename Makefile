CC := gcc
CFLAGS := -g -Wall -O3
LDFLAGS := -lm

.PHONY: test clean re

nn: main.o block.o instruction.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

nn-test: test/runner.o test/instruction.o block.o instruction.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lunity

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

instruction.o: instruction.c instruction.h instr/gemm.h instr/transpose.h

test: nn-test
	./nn-test

clean:
	rm -rf nn nn-test *.o

re: clean nn
