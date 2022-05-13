#ifndef __CLOG_H__
#define __CLOG_H__

#ifndef CLOG_DEFAULT_FMT
#define CLOG_DEFAULT_FMT "%t - %f:%n [%l]: %m\n"
#endif

#ifndef CLOG_DEFAULT_TIME_FMT
#define CLOG_DEFAULT_TIME_FMT "%Y-%m-%d_%H:%M:%S"
#endif

#ifndef CLOG_FORMAT_SIZE
#define CLOG_FORMAT_SIZE 32
#endif

#ifndef CLOG_MAX_LOG_SIZE
#define CLOG_MAX_LOG_SIZE 1024
#endif

#ifndef CLOG_GLOBALS
#define CLOG_GLOBALS 0
#endif

enum clog_level {
    CLOG_DEBUG = 4,	// use for debug printing
    CLOG_INFO  = 3,	// use for info logs, warnings and errors (default)
    CLOG_WARN  = 2,	// use for warnings and errors only
    CLOG_ERROR = 1,	// use for errors only
	CLOG_QUIET = 0	// use when no log output is desired
};

typedef struct clogger {
	enum clog_level level;
	char opened;
	int fd;
	char log_fmt[CLOG_FORMAT_SIZE];
	char time_fmt[CLOG_FORMAT_SIZE];
} clogger;

void clog_log(clogger* const logger, enum clog_level const lvl, char const * const file, int const line, char const * const fmt, ...);
#define log_err(logger, ...) clog_log(logger, CLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define elog(...) log_err(__VA_ARGS__)
#define log_warn(logger, ...) clog_log(logger, CLOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define wlog(...) log_warn(__VA_ARGS__)
#define log_info(logger, ...) clog_log(logger, CLOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define ilog(...) log_info(__VA_ARGS__)
#define log_dbg(logger, ...) clog_log(logger, CLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define dlog(...) log_dbg(__VA_ARGS__)

int clog_init_path(clogger* const logger, char const * const path);

int clog_init_fd(clogger* const logger, int const fd);

#ifdef __STDIO_H__
int clog_init_stream(clogger* const logger, FILE* const file);
#endif

int clog_close(clogger* const logger);

void clog_detach(clogger* const logger);

void clog_set_level(clogger* const logger, enum clog_level const lvl);

int clog_set_fmt(clogger* const logger, char* const fmt);

int clog_set_time_fmt(clogger* const logger, char* const fmt);

int clog_set_fd(clogger* const logger, int const fd);

#if CLOG_GLOBALS > 0
extern clogger _glob_clogs[CLOG_GLOBALS];

#define glog_err(nr, ...)  log_err(_glob_clogs + (nr), __VA_ARGS__)
#define eglog(...) glog_err(__VA_ARGS__)
#define glog_warn(nr, ...) log_warn(_glob_clogs + (nr), __VA_ARGS__)
#define wglog(...) glog_warn(__VA_ARGS__)
#define glog_info(nr, ...) log_info(_glob_clogs + (nr), __VA_ARGS__)
#define iglog(...) glog_info(__VA_ARGS__)
#define glog_dbg(nr, ...)  log_dbg(_glob_clogs + (nr), __VA_ARGS__)
#define dglog(...) glog_dbg(__VA_ARGS__)

#define gclog_init_path(nr, path) clog_init_path(_glob_clogs + (nr), path)

#define gclog_init_fd(nr, fd) clog_init_fd(_glob_clogs + (nr), fd)

#ifdef __STDIO_H__
#define gclog_init_stream(nr, stream) clog_init_path(_glob_clogs + (nr), stream)
#endif

#define gclog_close(nr) clog_close(_glob_clogs + (nr))

#define gclog_detach(nr) clog_detach(_glob_clogs + (nr))

#define gclog_set_level(nr, lvl) clog_set_level(_glob_clogs + (nr), lvl)

#define gclog_set_fmt(nr, fmt) clog_set_fmt(_glob_clogs + (nr), fmt)

#define gclog_set_time_fmt(nr, fmt) clog_set_time_fmt(_glob_clogs + (nr), fmt)

#define gclog_set_fd(nr, fd) clog_set_fd(_glob_clogs + (nr), fd)

#define gclog_getptr(nr) (_glob_clogs + (nr))

#endif /* if CLOG_GLOBALS > 0 */

#endif /* ifndef __CLOG_H__ */
