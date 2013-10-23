#include <stdlib.h> /* strtod */
#include "memory.h"
#include "str.h"

#include "value.h"
#include "basic.h"
#include "double.h"
#include "long.h"
#include "qstr.h"

value type_double(value f)
	{
	if (!f->N)
		{
		double *p = (double *)f->R;
		free_memory(p, sizeof(double));
		}
	return 0;
	}

value Qdouble(double val)
	{
	double *p = new_memory(sizeof(double));
	*p = val;
	return V(type_double,0,(value)p);
	}

double get_double(value f)
	{
	data_type(f,type_double);
	double *p = (double *)f->R;
	return *p;
	}

/* Convert string to double and return true if successful. */
int string_double(const char *beg, double *num)
	{
	char *end;
	*num = strtod(beg, &end);
	return *beg != '\0' && *end == '\0';
	}

static double op_add(double x, double y) { return x + y; }
static double op_sub(double x, double y) { return x - y; }
static double op_mul(double x, double y) { return x * y; }
/*
Note that dividing by zero is no problem here. If you divide a non-zero by
zero, it yields inf (infinity). If you divide zero by zero, it yields -nan
(not a number).
*/
static double op_div(double x, double y) { return x / y; }

static double op_cmp(double x, double y) { return x < y ? -1 : x > y ? 1 : 0; }

static value op2(value f, double op(double,double))
	{
	if (!f->L || !f->L->L) return 0;
	value x = eval(f->L->R);
	value y = eval(f->R);
	value z = Qdouble(op(get_double(x),get_double(y)));
	drop(x);
	drop(y);
	return z;
	}

value fexl_double_add(value f) { return op2(f,op_add); }
value fexl_double_sub(value f) { return op2(f,op_sub); }
value fexl_double_mul(value f) { return op2(f,op_mul); }
value fexl_double_div(value f) { return op2(f,op_div); }

/* (double_cmp x y) compares x and y and returns -1, 0, or +1. */
value fexl_double_cmp(value f) { return op2(f,op_cmp); }

/* double_string num */
extern int sprintf(char *str, const char *format, ...);
value fexl_double_string(value f)
	{
	if (!f->L) return 0;

	value x = eval(f->R);

	char buf[100]; /* being careful here */
	/* We show 15 digits because that's what Perl does. */
	sprintf(buf, "%.15g", get_double(x));

	value g = Qstr0(buf);
	drop(x);
	return g;
	}

value fexl_double_long(value f)
	{
	if (!f->L) return 0;
	value x = eval(f->R);
	value y = Qlong(get_double(x));
	drop(x);
	return y;
	}
