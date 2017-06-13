#include "chunks.h"

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	static struct argp_option options[] = {
	{"socket" ,				's',	"127.0.0.1:9000",	0,	"Socket addres with port",			0},
	{"threads" ,				't',	"2",				0,	"Count of using outputs threads",	0},
	{"file" ,					'f',	"/file/path",		0,	"File with graph paramethers",		0},
	{"offset" ,				'o',	"0",				0,	"Graph offset in pixels",			0},
	{"multiplier" ,			'm',	"1",				0,	"Multiplier for file paramether",		0},
	{"sleep" ,				'p',	"10",			0,	"Harvester thread sleep time",		0},
	{0}};

	struct argp argp	= {0};
	argp.options		= options;
	argp.parser		= parse_opt;

	argp_parse (&argp, argc, argv, 0, 0, NULL);

	if(FILE_PATH == NULL)
	{
		fprintf(stderr, "ERROR: --file should have argument\n");
		exit(EXIT_FAILURE);
	}

	APP app;
	app.socket		= -1;
	app.count		= 10*24;		// 10 px per hour
	app.offset		= OFFSET;
	app.multiplier	= MULTIPLIER;
	app.file_path		= FILE_PATH;
	app.sleep		= SLEEP;
	app.data			= malloc(app.count * sizeof(double));

	for(size_t i=0;i<app.count; i++)
	{
		app.data[i] = NAN;
	}

	pthread_mutex_init(&(app.accept_mutex), NULL);

	FCGX_Init();

	app.socket = FCGX_OpenSocket(SOCKET, 20);
	if(app.socket < 0)
	{
		fputs("ERROR: Failed to open socket\n", stderr);
		return 1;
	}

	pthread_t* threads = calloc(THREAD_COUNT, sizeof(pthread_t));

	pthread_t harv;
	pthread_create(&harv, NULL, harv_thread, &app);

	for(size_t i = 0; i < THREAD_COUNT; i++)
		pthread_create(threads+i, NULL, out_thread, &app);

	for(size_t i = 0; i < THREAD_COUNT; i++)
		pthread_join(threads[i], NULL);

	free(app.data);
	free(threads);

	return 0;
}
