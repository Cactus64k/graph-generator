#include "chunks.h"

void strcat_realloc(char** buff, char* str, intmax_t str_len, intmax_t* buff_size, intmax_t* buff_pos)
{
	assert(buff != NULL);
	assert(str != NULL);
	assert(buff_size != NULL);
	assert(buff_pos != NULL);

	if(*buff_size - *buff_pos - 1 > str_len)
	{
		strcat(*buff, str);
		*buff_pos = *buff_pos + str_len;
	}
	else
	{
		*buff_size = *buff_size * 2;
		*buff = realloc(*buff, *buff_size);
		strcat_realloc(buff, str, str_len, buff_size, buff_pos);
	}
}
