CLOGCONF=-DCLOG_GLOBALS=2
CCFLAGS=-Wall -Wextra -Wpedantic -std=c99 -g $(CLOGCONF)
LDFLAGS=-L. -lclog

ifndef verbose
.SILENT:
endif

default: clean ex_gcc clean ex_clang

ex_gcc: example.c libclog_gcc.a
	printf "GCC --------------------------\n"
	cc $< -o $@ $(CCFLAGS) $(LDFLAGS)_gcc
	printf "\n"
	./$@
	printf "\n\n"

ex_clang: example.c libclog_clang.a
	printf "CLANG ------------------------\n"
	clang $< -o $@ $(CCFLAGS) $(LDFLAGS)_clang
	printf "\n"
	./$@
	printf "\n\n"

libclog_gcc.a: clog.c clog.h
	cc -c $< -o clog.o $(CCFLAGS)
	ar -rcs $@ clog.o

libclog_clang.a: clog.c clog.h
	clang -c $< -o clog.o $(CCFLAGS)
	ar -rcs $@ clog.o

speed: speed.c libclog_gcc.a libclog_clang.a
	printf "GCC --------------------------\n"
	clang $< -o $@ $(CCFLAGS) $(LDFLAGS)_gcc
	/usr/bin/time ./$@
	printf "\nCLANG ------------------------\n"
	clang $< -o $@ $(CCFLAGS) $(LDFLAGS)_clang
	/usr/bin/time ./$@
	printf "\n"
	make clean

.PHONY: clean run default speed

clean:
	rm -f ex_* speed *.o *.a *.txt *.log
