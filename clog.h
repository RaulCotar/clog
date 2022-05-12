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

#ifndef CLOG_NO_PERR
#define _clog_perr(fmt, ...) fprintf(stderr, fmt __VA_OPT__(,) __VA_ARGS__)
#else
#define _clog_perr(fmt, ...) (void) fmt
#endif

void clog_log(clogger* const logger, enum clog_level const lvl, char const * const file, int const line, char const * const fmt, ...);
#define clog_err(logger, fmt, ...) clog_log(logger, CLOG_ERROR, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define clog_warn(logger, fmt, ...) clog_log(logger, CLOG_WARN, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define clog_info(logger, fmt, ...) clog_log(logger, CLOG_INFO, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define clog_dbg(logger, fmt, ...) clog_log(logger, CLOG_DEBUG, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)

int clog_init_path(clogger* const logger, char const * const path);

int clog_init_fd(clogger* const logger, int const fd);

#ifdef __STDIO_H__
int clog_init_stream(clogger* const logger, FILE* const file);
#endif

int clog_close(clogger* const logger);

void clog_set_level(clogger* const logger, enum clog_level const lvl);

int clog_set_fmt(clogger* const logger, char* const fmt, ...);

int clog_set_time_fmt(clogger* const logger, char* const fmt, ...);

#endif /* ifndef __CLOG_H__ */
