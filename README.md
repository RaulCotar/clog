# CLOG - a fast and simple logging library for small C projects

This library is meant for small to medium-sized projects that need a fast, simple and flexible
logging solution.

## Features:
- use any number of loggers
- log to any file descriptor
- arbitrary formatting
- multiple log levels

## Limitations:
- there is a (configurable) maximum size for a log
- logger lifetimes have to be handle manually
- thread safety is not (yet) guarranteed

## How to use:
- define configuration macros
- include the header
- link with libclog.a

## Configuration:
Configuring clog is as simple as (not)defining any of the following macros when compiling libclog:
- `CLOG_NO_PERR`: disables internal errors being printed to stderr (default: undefined)
- `CLOG_DEFAULT_FMT`: the defalut format for the log messages
- `CLOG_DEFAULT_TIME_FMT`: the default format for timestamps used in logs
- `CLOG_FORMAT_SIZE`: the maximum length of a format string (default: 32) (not that important)
- `CLOG_MAX_LOG_SIZE`: the maximum size of a log (try to keep it under 4KiB, default: 1KiB)
- `CLOG_DEFAULT_LEVEL`: the default log level when initializing a clogger (default: INFO)
