#include "chunks.h"

void* out_thread(void* ptr)
{
	APP* app							= (APP*) ptr;
	pthread_mutex_t* accept_mutex	= &(app->accept_mutex);

	FCGX_Request request;

	if(FCGX_InitRequest(&request, app->socket, 0) != 0)
	{
		fputs("ERROR: Can not init request\n", stderr);
		return NULL;
	}

	intmax_t buff_size	= 4096;
	char* buff		= malloc(buff_size);

	while(1)
	{
		pthread_mutex_lock(accept_mutex);
		int rc = FCGX_Accept_r(&request);
		pthread_mutex_unlock(accept_mutex);

		if(rc == 0)
		{
			polyline_generate(&buff, &buff_size, app->data, app->count, app->offset);
			FCGX_PutS("Content-type: image/svg+xml\r\n\r\n", request.out);
			FCGX_PutS(buff, request.out);
		}
		else
		{
			fputs("ERROR: Can not accept new request\n", stderr);
			break;
		}

		FCGX_Finish_r(&request);
	}

	free(buff);

	return NULL;
}
