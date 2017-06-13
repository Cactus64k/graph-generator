#include "chunks.h"

void* harv_thread(void* ptr)
{
	APP* app		= (APP*)ptr;
	int period	= DAYSEC / app->count;

	while(1)
	{
		int fd	= open(app->file_path, O_RDONLY);
		FILE* f	= fdopen(fd, "rb");

		if(f != NULL)
		{
			fd_set rfds;
			int timeout		= period - time(NULL)%period;
			struct timeval tv	= {.tv_sec = timeout, .tv_usec = 0};
			FD_ZERO(&rfds);
			FD_SET(fd, &rfds);

			double value = 0;

			if(	(select(fd+1, &rfds, NULL, NULL, &tv) != -1)
				&&
				(fscanf(f, "%lf", &value) == 1))
			{
				size_t pos		= (time(NULL) % DAYSEC)/period;
				value			= value * app->multiplier;

				if(isnan(app->data[pos]) || app->data[pos] == 0)
					app->data[pos] = value;
				else
					app->data[pos] = (app->data[pos] + value)/2;

				if(pos+1 < app->count)
					app->data[pos+1]	= NAN;
			}
			else
			{
				size_t pos		= (time(NULL) % DAYSEC)/period;
				app->data[pos]	= NAN;
			}

			fclose(f);
		}

		sleep(2);
	}
	return NULL;
}


