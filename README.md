# CLOG - a fast and simple logging library for small C projects

This library is meant for small to medium-sized projects that need a fast, simple and flexible
logging solution. Due to it's simplicity, clog is also extensible, so custom features can be added relatively easily.

## Features:
- Use any number of loggers
- Log to any file descriptor
- Arbitrary log formatting
- Multiple log levels
- High throughput (~240k logs/sec on i5-2430M, SATA3 ssd, Arch Linux)

## Limitations:
- There is a (configurable) maximum size for a log
- Thread safety is not (yet) guarranteed
- Requires C99 or higher, with some posix functions. Feature test: `_STDC_VERSION__ >= 199901L && _POSIX_C_SOURCE >= 200809L && _POSIX_VERSION >= 200809L`
- Officially supports only GCC 12 and Clang 13

## Usage:
- Compile `libclog` with the desired configuration macros
- Include `clog.h` wherever you need logging
- Link with `libclog` (static linking is preferred for speed)
- See `test.c` for examples on how to use clog.

## Configuration:
- Important:
	- `CLOG_NO_PERR` disables internal errors being printed to stderr (default: undefined)
	- `CLOG_DEFAULT_FMT <string>` the defalut format for the log messages
	- `CLOG_DEFAULT_TIME_FMT <string>` the default format for timestamps used in logs
	- `CLOG_MAX_LOG_SIZE <size>` the maximum size of a log (default: 1KiB)
	- `CLOG_DEFAULT_LEVEL <enum>` the default log level when initializing a logger (default: CLOG_INFO)
	- `CLOG_GLOBALS <count>` defines global loggers and convenience macros to use them (default: 0) (must be defined with the same value in all translation units that use global loggers)
- Extra:
	- `CLOG_FORMAT_SIZE <size>` the maximum length of a format string (default: 32)
	- `CLOG_OPEN_FILE_MODE <mode_t>` the access mode of a newly created log file (default: 0420)

## Future plans:
Testing and bug fixing are now the main prioriy, but new features are also being considered:
- Windows support (MSVC and/or MingW)
- thread safety (logging should be doable from multiple threads in parallel)
- configurable log levels
- logger identifiers (loggers will be referenced by their id instead of pointers or numbers)
- log function that can handle big logs (likely using heap allocations)
- nicer internal error handling (more detailed, configurable and reliable)
