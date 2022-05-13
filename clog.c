#if _POSIX_C_SOURCE < 200809L
#define _POSIX_C_SOURCE 200809L
#endif

#include "clog.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#define CLOG_SBUF_SIZE ((size_t)CLOG_MAX_LOG_SIZE)
#define CLOG_FMT_SIZE ((size_t)CLOG_FORMAT_SIZE)

#ifndef CLOG_DEFAULT_LEVEL
#define CLOG_DEFAULT_LEVEL CLOG_INFO
#endif

#ifndef CLOG_NO_PERR
#define _clog_perr(...) fprintf(stderr, "!clog! "__VA_ARGS__)
#else
#define _clog_perr(fmt, ...) ((void) fmt)
#endif

#ifndef CLOG_OPEN_FILE_MODE
#define CLOG_OPEN_FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#endif

#if CLOG_GLOBALS > 0
clogger _glob_clogs[CLOG_GLOBALS];
#endif

#define _buff_free_space (CLOG_SBUF_SIZE + sbuff - head)
#define _clog_fmt_time(buff, head, fmt, timefn) \
    if (logger->time_fmt[0] != 0) { \
        time_t t = time(NULL); \
        struct tm tm; \
        head += strftime(head, _buff_free_space, logger->time_fmt, timefn(&t, &tm)); \
    }
void clog_log(clogger* const logger, enum clog_level const lvl,
		char const * const file, int const line, const char* const fmt, ...)
{
	if (!logger) {
		_clog_perr("Invalid logger (NULL)!\n");
		return;
	}
	if (lvl > logger->level || logger->log_fmt[0] == 0)
		return;
	if (logger->status != 1) {
		_clog_perr("Cannot use logger with status %d!", logger->status);
		return;
	}

	va_list ap;
	va_start(ap, fmt);
	
	char sbuff[CLOG_SBUF_SIZE];
	char* head = sbuff;
	for (size_t i=0; i<CLOG_FMT_SIZE && logger->log_fmt[i]!=0; i++) {
        if (logger->log_fmt[i] != '%')
            *head++ = logger->log_fmt[i];
        else {
            switch (logger->log_fmt[++i]) {
            	case 'f':
            		head = stpncpy(head, file, _buff_free_space);
            		break;
            	case 'n':
            		head += snprintf(head, _buff_free_space, "%d", line);
            		break;
                case 'l':
            		head = stpncpy(head, "QUIET\0ERROR\0WARN\0\0INFO\0\0DEBUG" + ((lvl % 5) * 6),
            				_buff_free_space);
            		break;
            	case 't':
            		_clog_fmt_time(sbuff, head, logger->time_fmt, gmtime_r);
            		break;
            	case 'T':
            		_clog_fmt_time(sbuff, head, logger->time_fmt, localtime_r);
            		break;
            	case 'm':
					head += vsnprintf(head, _buff_free_space, fmt, ap);
            		break;
            	case '%':
            		*head++ = '%';
            		break;
            	default:
            		_clog_perr("Invalid log format string! '%%%c'\n", logger->log_fmt[i]);
            		i = CLOG_FMT_SIZE; // exits the for loop
            		break;
            }
        }
    }
	va_end(ap);

	if (write(logger->fd, sbuff, head-sbuff) < 0)
        _clog_perr("Error while flushing buffer to fd! ERRNO %d: %s\n", errno, strerror(errno));
}
#undef _buff_free_space
#undef _clog_fmt_time

void clog_set_level(clogger* const logger, enum clog_level const lvl) {
	if (lvl < CLOG_QUIET || lvl > CLOG_DEBUG)
		_clog_perr("Invalid log level: %d\n", lvl);
	else
		logger->level = lvl;
}

int clog_set_fmt(clogger* const logger, char* const fmt) {
	if (strlen(fmt) > CLOG_FMT_SIZE) {
		_clog_perr("Log format string too large.");
		return -1;
	}
	strcpy(logger->log_fmt, fmt);
	return 0;
}

int clog_set_time_fmt(clogger* const logger, char* const fmt) {
	if (strlen(fmt) > CLOG_FMT_SIZE) {
		_clog_perr("Time format string too large.");
		return -1;
	}
	strcpy(logger->time_fmt, fmt);
	return 0;
}

int clog_set_fd(clogger* const logger, int const fd) {
	if (fd < 0) {
		_clog_perr("Trying to set invalid file descriptor %d.\n", fd);
		return fd;
	}
	logger->fd = fd;
	logger->status = 1;
	return 0;
}

int clog_init_path(clogger* const logger, char const * const path) {
	int fd = open(path, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC | O_DSYNC, CLOG_OPEN_FILE_MODE);
	if (fd == -1) {
		_clog_perr("Cannot open '%s'. ERRNO %d: %s\n", path, errno, strerror(errno));
		return fd;
	}
	return clog_init_fd(logger, fd);
}

int clog_init_stream(clogger* const logger, FILE* const stream) {
	int fd = fileno(stream);
	if (fd == -1) {
		_clog_perr("Cannot obtain file descriptor from stream. ERRNO %d: %s\n",
				errno, strerror(errno));
		return fd;
	}
	return clog_init_fd(logger, fd);
}

int clog_init_fd(clogger* const logger, int const fd) {
	if (fd < 0) {
		_clog_perr("Trying to initialize logger with invalid file descriptor %d.\n", fd);
		return fd;
	}
	logger->fd = fd;
	logger->level = CLOG_DEFAULT_LEVEL;
	logger->status = 1;
	strcpy(logger->log_fmt, CLOG_DEFAULT_FMT);
	strcpy(logger->time_fmt, CLOG_DEFAULT_TIME_FMT);
	return 0;
}

int clog_close(clogger* const logger) {
	if (logger->status == 0) {
		_clog_perr("Logger with fd %d has status 0.\n", logger->fd);
		return 1;
	}
	int r = close(logger->fd);
	logger->status = 0;
	if (r == -1) {
		_clog_perr("Couldn't close file descriptor %i. ERRNO %d: %s\n", logger->fd,
				errno, strerror(errno));
		return -1;
	}
	return 0;
}

void clog_detach(clogger* const logger) {
	logger->fd = 0;
	logger->status = 0;
}
