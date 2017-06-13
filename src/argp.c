#include "chunks.h"

char* SOCKET			= "127.0.0.1:9000";
int THREAD_COUNT		= 2;
char* FILE_PATH			= NULL;
double OFFSET			= 0.f;
double MULTIPLIER		= 1.f;
int SLEEP				= 10;

int parse_opt(int key, char* arg, struct argp_state* state)
{
	switch (key)
	{
		case 's':
		{
			size_t arg_len = strlen(arg);
			if(arg_len != 0)
			{
				SOCKET = malloc(arg_len + 1);
				strncpy(SOCKET, arg, arg_len);
			}
			break;
		}
		case 't':
		{
			if(sscanf(arg, "%d", &THREAD_COUNT) != 1)
			{
				fprintf(stderr, "\nERROR: Failed to parse threads count %s\n", arg);
				exit(EXIT_FAILURE);
			}

			if(THREAD_COUNT > 64)
			{
				fprintf(stderr, "\nERROR: Too many threads. The maximum number of threads is 64\n");
				exit(EXIT_FAILURE);
			}
			break;
		}
		case 'f':
		{
			struct stat s;
			if(stat(arg, &s) == -1)
			{
				fprintf(stderr, "\nERROR: File %s is not exist\n", arg);
				exit(EXIT_FAILURE);
			}
			else
			{
				if(S_ISREG(s.st_mode) || S_ISCHR(s.st_mode))
				{
					size_t arg_len = strlen(arg);
					FILE_PATH = malloc(arg_len+1);
					strncpy(FILE_PATH, arg, arg_len);
				} else
				{
					fprintf(stderr, "\nERROR: %s is not a file\n", arg);
					exit(EXIT_FAILURE);
				}
			}
			break;
		}
		case 'o':
		{
			if(sscanf(arg, "%lf", &OFFSET) != 1)
			{
				fprintf(stderr, "\nERROR: Failed to parse offset %s\n", arg);
				exit(EXIT_FAILURE);
			}

			break;
		}
		case 'm':
		{
			if(sscanf(arg, "%lf", &MULTIPLIER) != 1)
			{
				fprintf(stderr, "\nERROR: Failed to parse multiplier %s\n", arg);
				exit(EXIT_FAILURE);
			}

			break;
		}
		case 'p':
		{
			if(sscanf(arg, "%d", &SLEEP) != 1)
			{
				fprintf(stderr, "\nERROR: Failed to parse sleep %s\n", arg);
				exit(EXIT_FAILURE);
			}

			break;
		}
	}
	return 0;
}
