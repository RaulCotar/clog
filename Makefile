CCFLAGS=-Wall -Wextra -Wpedantic -std=gnu2x -g
LDFLAGS=-L. -lclog

run: test
	./$<
	cat log.txt
	rm log.txt

test: main.c libclog.a
	cc $< -o $@ $(CCFLAGS) $(LDFLAGS)

libclog.a: clog.c clog.h
	cc -c $< -o clog.o $(CCFLAGS)
	ar -rcs $@ clog.o

pp: clog.c clog.h
	cc $(CCFLAGS) -E $< -o $@

.PHONY: clean run

clean:
	rm -f test *.o *.txt *.a pp
