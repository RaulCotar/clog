#include "clog.h"

clogger l;

int main() {
	clog_init_path(&l, "log.txt");

	clog_dbg(&l, "debug");
	clog_info(&l, "info");
	clog_warn(&l, "warning");
	clog_err(&l, "error");

	clog_close(&l);
	return 0;
}
