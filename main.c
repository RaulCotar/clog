#include "clog.h"

clogger l;
#define log(...) clog_info(&l, __VA_ARGS__)

int main() {
	clog_init_path(&l, "log.txt");

	for(int i=0; i<1000000; i++) {
		log("Short log message %d.", i);
	}

	char* string = "This string will get copied to every log message below.";
	for(int i=0; i<1000000; i++) {
		log("log no. %d: %s", i, string);
	}

	clog_close(&l);
	return 0;
}
