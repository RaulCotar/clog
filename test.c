/*
 * This file is meant to test the clog library and possibly provide more examples of how to use it.
 * In the future refactor using Arav's testing header.
 */
#define _POSIX_C_SOURCE_ 200809L
#include <pthread.h>
#include <stdio.h>
#include "clog.h"

#define test(call) \
	do { \
		int result = (call); \
		if (result) { \
			fprintf(stderr, "Test " #call " failed with return code %d\n", result); \
			failed++; \
		} \
	} while (0)

void* threadlog(void* arg)
{
	log_info((clogger*)arg, "Logging from a pthread.");
	return NULL;
}

int threading()
{
	pthread_t threads[4];
	clogger logger;
	clog_init_path(&logger, "threading.log");
	int ret = 0;

	log_info(&logger, "Starting 4 logging threads.");
	for (int i=0; i<4; i++)
	{
		int c = pthread_create(&threads[i], NULL, threadlog, &logger);
		if (c)
		{
			perror("Error while creating a thread!");
			ret = 1;
		}
	}
	for (int i=0; i<4; i++)
	{
		int c = pthread_join(threads[i], NULL);
		if (c)
		{
			perror("Error while joining a thread!");
			ret = 2;
		}
	}
	log_info(&logger, "All threads terminated successfully");

	if(clog_close(&logger))
		ret = 3;

	return ret;
}

int main()
//int main(int argc, char** argv)
{
	int failed = 0;

	test(threading());

	if (!failed)
		puts("All tests passed!");
	else
		printf("%d tests failed!\n", failed);

	return 0;
}
