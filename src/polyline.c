#include "chunks.h"

#define SVG_HEADER "" \
				"<svg xmlns='http://www.w3.org/2000/svg' height='100%' width='100%' version='1.1' viewBox='0 0 270 193'>\r\n" \
				"<g transform='translate(20, 110) scale(1, -1)' stroke='#d07735' stroke-width='0.5'>\r\n"

#define SVG_POLYLINE_START		"<polyline fill='none' points='"
#define SVG_POLYLINE_END		"'/>\r\n"
#define SVG_TIME_TEAMPLATE		"<polyline stroke-opacity='0.3' fill='red' stroke='red' transform='translate(%d, 0)' points='0,0 1,-2 -1,-2 0,0 0,100'/>\r\n"
#define SVG_END					"</g>\r\n</svg>\r\n"


void polyline_point_add(char** buff, intmax_t* buff_size, intmax_t* buff_pos, size_t pos, double value)
{
	char point[16];
	snprintf(point, sizeof(point), "%d,%.3lf ", (int)pos, value);
	strcat_realloc(buff, point, strlen(point), buff_size, buff_pos);
}

void polyline_generate(char** buff, intmax_t* buff_size, double* graph, size_t count, double offset)
{
	assert(buff != NULL);
	assert(*buff != NULL);
	assert(buff_size != NULL);
	assert(graph != NULL);

	**buff				= '\0';
	intmax_t buff_pos	= 0;

	strcat_realloc(buff, SVG_HEADER, sizeof(SVG_HEADER)-1, buff_size, &buff_pos);
	for(size_t i=0; i<count; i++)
	{
		if(!isnan(graph[i]))
		{
			strcat_realloc(buff, SVG_POLYLINE_START, sizeof(SVG_POLYLINE_START)-1, buff_size, &buff_pos);
			for(; i<count; i++)
			{
				if(isnan(graph[i]))
					break;
				else
					polyline_point_add(buff, buff_size, &buff_pos, i , graph[i] + offset);
			}
			strcat_realloc(buff, SVG_POLYLINE_END, sizeof(SVG_POLYLINE_END)-1, buff_size, &buff_pos);
		}
	}

	char time_buff[sizeof(SVG_TIME_TEAMPLATE) + 8];
	size_t pos		= (time(NULL) % DAYSEC)/(60*6);
	snprintf(time_buff, sizeof(time_buff), SVG_TIME_TEAMPLATE, (int)pos);
	strcat_realloc(buff, time_buff, strlen(time_buff), buff_size, &buff_pos);

	strcat_realloc(buff, SVG_END, sizeof(SVG_END)-1, buff_size, &buff_pos);
}
