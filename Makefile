CC := gcc
CFLAGS := -fuse-ld=mold -fopenmp -g -Wall -O3
LDFLAGS := -lm

.PHONY: test clean re

nn: main.o vec.o image.o block.o instruction.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

nn-omp: main.o vec.o image.o block.o instruction-omp.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

nn-test: test/runner.o test/vec.o test/instruction.o vec.o block.o instruction.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lunity

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

instruction-omp.o: instruction.c instruction.h instr/gemm.h instr/transpose.h
	$(CC) $(CFLAGS) -DINSTR_OMP -c $< -o $@

instruction.o: instruction.c instruction.h instr/gemm.h instr/transpose.h

test: nn-test
	./nn-test

clean:
	rm -rf nn nn-test *.o

re: clean nn
