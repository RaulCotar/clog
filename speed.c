#include "clog.h"

int main() {
	clogger l;
	clog_init_path(&l, "throuput.log");
	for (int i=0; i<1000000; i++) {
		log_info(&l, "Testing logger throughput (str: '%s') (iter: %i)", "some message", i);
	}
	clog_close(&l);
	return 0;
}

