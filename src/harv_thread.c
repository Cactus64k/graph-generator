#include "chunks.h"

size_t get_day_pos(size_t period)
{

	time_t t = time(NULL);

	struct tm* ts = localtime(&t);

	time_t sec		= (ts->tm_hour)*60*60 + (ts->tm_min)*60 + ts->tm_sec;
	size_t pos		= sec/period;

	return pos;
}

void* harv_thread(void* ptr)
{
	APP* app		= (APP*) ptr;

	while(1)
	{
		size_t pos		= get_day_pos(GRAPH_PERIOD);
		FILE* f			= fopen(app->file_path, "rb");
		if(f != NULL)
		{
			double value	= 0;

			if(fscanf(f, "%lf", &value) == 1)
			{
				value = value * app->multiplier;
				if(app->data[pos] != 0)
					app->data[pos] = (app->data[pos] + value)/2;
				else
					app->data[pos] = value;
			}

			fclose(f);
		}

		sleep(app->sleep);
	}
	return NULL;
}


