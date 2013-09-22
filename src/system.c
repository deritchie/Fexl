#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include "value.h"
#include "long.h"
#include "system.h"

/* exit status : exits with the given status */
static value type_exit(value f)
	{
	if (!f->L) return 0;
	value x = arg(type_long,f->R);
	long status = long_val(x);
	exit(status);
	return 0;
	}

/* (setrlimit resource soft hard) = status */
static value type_setrlimit(value f)
	{
	if (!f->L || !f->L->L || !f->L->L->L) return 0;

	value x = arg(type_long,f->L->L->R);
	value y = arg(type_long,f->L->R);
	value z = arg(type_long,f->R);

	long resource = long_val(x);
	struct rlimit rlim;
	rlim.rlim_cur = long_val(y);
	rlim.rlim_max = long_val(z);

	int status = setrlimit(resource, &rlim);

	check(x);
	check(y);
	check(z);

	return Qlong(status);
	}

static long get_RLIMIT(const char *name, int *ok)
	{
	*ok = 1;
	if (strcmp(name,"AS") == 0) return RLIMIT_AS;
	if (strcmp(name,"CORE") == 0) return RLIMIT_CORE;
	if (strcmp(name,"CPU") == 0) return RLIMIT_CPU;
	if (strcmp(name,"DATA") == 0) return RLIMIT_DATA;
	if (strcmp(name,"FSIZE") == 0) return RLIMIT_FSIZE;
	if (strcmp(name,"LOCKS") == 0) return RLIMIT_LOCKS;
	if (strcmp(name,"MEMLOCK") == 0) return RLIMIT_MEMLOCK;
	if (strcmp(name,"MSGQUEUE") == 0) return RLIMIT_MSGQUEUE;
	if (strcmp(name,"NICE") == 0) return RLIMIT_NICE;
	if (strcmp(name,"NOFILE") == 0) return RLIMIT_NOFILE;
	if (strcmp(name,"NPROC") == 0) return RLIMIT_NPROC;
	if (strcmp(name,"RSS") == 0) return RLIMIT_RSS;
	if (strcmp(name,"RTPRIO") == 0) return RLIMIT_RTPRIO;
	if (strcmp(name,"RTTIME") == 0) return RLIMIT_RTTIME;
	if (strcmp(name,"SIGPENDING") == 0) return RLIMIT_SIGPENDING;
	if (strcmp(name,"STACK") == 0) return RLIMIT_STACK;
	*ok = 0;
	return -1;
	}

static value resolve_RLIMIT(const char *name)
	{
	int ok;
	long limit = get_RLIMIT(name,&ok);
	return ok ? Qlong(limit) : 0;
	}

value resolve_system(const char *name)
	{
	if (strncmp(name,"RLIMIT_",7) == 0) return resolve_RLIMIT(name+7);
	if (strcmp(name,"setrlimit") == 0) return Q(type_setrlimit);
	if (strcmp(name,"exit") == 0) return Q(type_exit);
	return 0;
	}

/* LATER more functions
if (strcmp(name,"argc") == 0) return Q(type_argc);
if (strcmp(name,"argv") == 0) return Q(type_argv);
if (strcmp(name,"getenv") == 0) return Q(type_getenv);
if (strcmp(name,"setenv") == 0) return Q(type_setenv);
*/
