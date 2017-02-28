#include "chunks.h"

int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	static struct argp_option options[] = {
	{"socket" ,				's',	"127.0.0.1:9000",	0,	"Socket addres with port",			0},
	{"threads" ,			't',	"4",				0,	"Count of using outputs threads",	0},
	{"file" ,				'f',	"/file/path",		0,	"File with graph paramethers",		0},
	{"offset" ,				'o',	"0",				0,	"Graph offset in pixels",			0},
	{"multiplier" ,			'm',	"1",				0,	"Multiplier for file paramether",	0},
	{"sleep" ,				'p',	"30",				0,	"Harvester thread sleep time",		0},
	{"fill" ,				'c',	"true",				0,	"Fill svg graph",					0},
	{0}};

	struct argp argp = {0};
	argp.options	= options;
	argp.parser		= parse_opt;
//	argp.doc		=	"Machine list format:"
//						"\n\nlogin:password@hostname\n"
//						"login2:password2@hostname2\n\n"
//						"Or login@hostname if you have public key authorisation. Each machine on a new line. Offline machines handled in main thread.";

	argp_parse (&argp, argc, argv, 0, 0, NULL);

	if(FILE_PATH == NULL)
	{
		fprintf(stderr, "ERROR: --file should have argument\n");
		exit(EXIT_FAILURE);
	}

	APP app;
	app.socket		= -1;
	app.count		= 60/6*24;
	app.offset		= OFFSET;
	app.multiplier	= MULTIPLIER;
	app.file_path	= FILE_PATH;
	app.sleep		= SLEEP;
	app.fill		= FILL;
	app.data		= calloc(app.count, sizeof(double));

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
