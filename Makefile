CCFLAGS=-Wall -Wextra -Wpedantic -std=c17 -g
LDFLAGS=-L. -lclog

run: test
	./$<

test: main.c libclog.a
	cc $< -o $@ $(CCFLAGS) $(LDFLAGS)

libclog.a: clog.c clog.h
	cc -c $< -o clog.o $(CCFLAGS)
	ar -rcs $@ clog.o

.PHONY: clean run

clean:
	rm -f test *.o *.txt *.a pp
