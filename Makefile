CC := gcc
CFLAGS := -fuse-ld=mold -fopenmp -g -Wall -O3
LDFLAGS := -lm

INSTR_HEADERS := instruction.h instr/softmax.h instr/util.h instr/gemm.h instr/transpose.h instr/relu.h

.PHONY: all test clean re

all: nn nn-omp nn-ikj

nn: main.o vec.o image.o block.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

nn-omp: main-omp.o vec.o image.o block.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

nn-ikj: main-ikj.o vec.o image.o block.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

nn-test: test/runner.o test/vec.o test/instruction.o vec.o block.o instruction.o nn.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lunity

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c block.h image.h nn.h vec.h $(INSTR_HEADERS)

main-omp.o: main.c block.h image.h nn.h vec.h $(INSTR_HEADERS)
	$(CC) $(CFLAGS) -DINSTR_OMP -c $< -o $@

main-ikj.o: main.c block.h image.h nn.h vec.h $(INSTR_HEADERS)
	$(CC) $(CFLAGS) -DINSTR_IKJ -c $< -o $@

instruction.o: instruction.c $(INSTR_HEADERS)

test: nn-test
	./nn-test

clean:
	rm -rf nn nn-test *.o

re: clean nn
