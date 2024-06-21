/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <time.h>
#include <unistd.h>
#include "bakery.h"
void
bakery_prog_1(char *host)
{
	CLIENT *clnt;
	struct BAKERY  *result_1;
	struct BAKERY  get_number_1_arg;
	struct BAKERY  *result_2;
	struct BAKERY  bakery_service_1_arg;
	clock_t total_t = 0, t;
	clnt = clnt_create (host, BAKERY_PROG, BAKERY_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	srand(time(NULL));
	usleep(rand() % 3000000);
	t = clock();
	result_1 = get_number_1(&get_number_1_arg, clnt);
	if (result_1 == (struct BAKERY *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	total_t += clock() - t;
	printf("Client id = %d, num = %d\n", result_1->pid, result_1->num);
	bakery_service_1_arg.pid = result_1->pid;
	bakery_service_1_arg.num = result_1->num;
	usleep(rand() % 3000000);
	t = clock();
	sleep(10);
	result_2 = bakery_service_1(&bakery_service_1_arg, clnt);
	if (result_2 == (struct BAKERY *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	total_t += clock() - t;
	printf("Client receive %c\n", result_2->res);
	printf("Total time: %lf s\n", (double)total_t / CLOCKS_PER_SEC);
	clnt_destroy (clnt);
}
int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	bakery_prog_1 (host);
	exit (0);
}