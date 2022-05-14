#include <stdio.h> // exposes clog_init_stream in clog.h

// CLOG_GLOBALS is specified with the -DCLOG_GLOBALS=2 flag
#include "clog.h"

int main() {

	/* using gobally defined loggers */
	gclog_init_path(0, "log.txt");
	gclog_init_fd(1, gclog_getptr(0)->fd); // don't use glog_getptr if you don't have to
	gclog_set_level(1, CLOG_WARN);
	iglog(0, "Hello World!");
	glog_info(0, "Hello World, again!");
	iglog(1, "This will not be logged");
	wglog(1, "Warning from global logger #%d", 1);

	/* using user-created loggers */
	clogger l;
	clog_init_fd(&l, fileno(stdout));
	clog_set_level(&l, CLOG_DEBUG);
	ilog(&l, "<- default formatting");
	clog_set_fmt(&l, "[custom formatting] %m\n");
	log_dbg(&l, "Debug message 1.");
	dlog(&l, "Check out log.txt.");
	clog_detach(&l);
	clog_set_fd(&l, gclog_getptr(0)->fd);
	ilog(&l, "Info from local logger.");
	
	clog_close(&l);
	gclog_detach(0); // fd already closed, so just detach
	gclog_detach(1); // -..-
	
	return 0;
}
