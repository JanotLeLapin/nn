CC := gcc
CFLAGS := -g -Wall

.PHONY: clean re

nn: main.o block.o
	$(CC) $(CFLAGS) $^ -o $@  -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf nn *.o

re: clean nn
