CCFLAGS=-Wall -Wextra -Wpedantic -std=c99 -g
LDFLAGS=-L. -lclog

run: test
	./$<

test: test.c libclog.a
	cc $< -o $@ $(CCFLAGS) $(LDFLAGS)

libclog.a: clog.c clog.h
	cc -c $< -o clog.o $(CCFLAGS)
	ar -rcs $@ clog.o

.PHONY: clean run

clean:
	rm -f test *.o *.txt *.a pp
