#include "memory.h"
#include "str.h"

#include "value.h"
#include "basic.h"
#include "qstr.h"

/*LATER wrap a combinator around the buf.c module, which will allow very fast
large-scale buffering */

value type_string(value f)
	{
	if (!f->N) str_free(get_str(f));
	return f;
	}

value Qstr(struct str *p)
	{
	return V(type_string,0,(value)p);
	}

value Qstr0(const char *data)
	{
	return Qstr(str_new_data0(data));
	}

struct str *get_str(value f)
	{
	data_type(f,type_string);
	return (struct str *)f->R;
	}

/* (string_append x y) is the concanation of strings x and y. */
value type_string_append(value f)
	{
	if (!f->L || !f->L->L) return f;
	value x = eval(f->L->R);
	value y = eval(f->R);
	value z = Qstr(str_append(get_str(x),get_str(y)));
	drop(x);
	drop(y);
	return z;
	}
