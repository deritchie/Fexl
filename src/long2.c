#include "str.h"

#include "value.h"
#include "long.h"
#include "qstr.h"

static long op_add(long x, long y) { return x + y; }
static long op_sub(long x, long y) { return x - y; }
static long op_mul(long x, long y) { return x * y; }
/* op_div returns 0 if you try to divide by 0. */
static long op_div(long x, long y) { return y ? x / y : 0; }

static value op2(value f, long op(long,long))
	{
	if (!f->L || !f->L->L) return f;
	value x = eval(f->L->R);
	value y = eval(f->R);
	value z = Qlong(op(get_long(x),get_long(y)));
	drop(x);
	drop(y);
	return z;
	}

value type_long_add(value f) { return op2(f,op_add); }
value type_long_sub(value f) { return op2(f,op_sub); }
value type_long_mul(value f) { return op2(f,op_mul); }
value type_long_div(value f) { return op2(f,op_div); }

/* Convert long to string. */
extern int sprintf(char *str, const char *format, ...);
value type_long_string(value f)
	{
	if (!f->L) return f;

	value x = eval(f->R);
	char buf[100]; /* being careful here */
	sprintf(buf, "%ld", get_long(x));

	value y = Qstr0(buf);
	drop(x);
	return y;
	}

/*TODO more functions */
#if 0
is_long
long_double
long_char
long_cmp
order
#endif