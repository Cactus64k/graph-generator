
#ifndef CHUNKS_H_
	#define CHUNKS_H_

	#define _POSIX_SOURCE

	#include <stdlib.h>
	#include <stdio.h>
	#include <stdint.h>
	#include <math.h>
	#include <string.h>
	#include <assert.h>
	#include <stddef.h>
	#include <time.h>
	#include <stdbool.h>

	#include <argp.h>
	#include <pthread.h>
	#include <fcgi_config.h>
	#include <fcgiapp.h>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <sys/time.h>
	#include <sys/types.h>

	extern char* SOCKET;
	extern int THREAD_COUNT;
	extern char* FILE_PATH;
	extern double OFFSET;
	extern double MULTIPLIER;
	extern int SLEEP;

	#define DAYSEC (24*60*60)

	void* out_thread(void* ptr);
	void* harv_thread(void* ptr);
	void strcat_realloc(char** buff, char* str, intmax_t str_len, intmax_t* buff_size, intmax_t* buff_pos);
	void polyline_generate(char** buff, intmax_t* buff_size, double* graph, size_t count, double offset);
	size_t get_day_pos(size_t period_min);
	int parse_opt (int key, char* arg, struct argp_state* state);

	typedef struct APP
	{
		pthread_mutex_t accept_mutex;
		int socket;
		char* file_path;
		size_t sleep;
		double multiplier;
		double offset;

		double* data;
		size_t count;
	} APP;

#endif /* CHUNKS_H_ */
