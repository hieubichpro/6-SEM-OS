/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "bakery.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

struct BAKERY *
get_number_1(struct BAKERY *argp, CLIENT *clnt)
{
	static struct BAKERY clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, GET_NUMBER,
		(xdrproc_t) xdr_BAKERY, (caddr_t) argp,
		(xdrproc_t) xdr_BAKERY, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct BAKERY *
bakery_service_1(struct BAKERY *argp, CLIENT *clnt)
{
	static struct BAKERY clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BAKERY_SERVICE,
		(xdrproc_t) xdr_BAKERY, (caddr_t) argp,
		(xdrproc_t) xdr_BAKERY, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
