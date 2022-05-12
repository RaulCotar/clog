#include "clog.h"

clogger l;
#define log(...) clog_info(&l, __VA_ARGS__)

int main() {
	clog_init_path(&l, "log.txt");

	log("one line log");
	log("one line log");

	clog_close(&l);
	return 0;
}
