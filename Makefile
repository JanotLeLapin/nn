CC := gcc
CFLAGS := -g -Wall
LDFLAGS := -lm

.PHONY: clean re

nn: main.o block.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf nn *.o

re: clean nn
