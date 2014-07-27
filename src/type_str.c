#include <value.h>
#include <basic.h>
#include <num.h>
#include <str.h>
#include <type_num.h>
#include <type_str.h>

value type_str(value f)
	{
	if (f->N == 0) str_free((string)f->R);
	return 0;
	}

value Qstr(string x)
	{
	if (!x) return 0;
	{
	value f = D(type_str,x);
	if (!f) str_free(x);
	return f;
	}
	}

/* (. x y) is the concatenation of strings x and y. */
value type_concat(value f)
	{
	if (!f->L || !f->L->L) return 0;
	{
	string x = atom(type_str,arg(&f->L->R));
	string y = atom(type_str,arg(&f->R));
	if (!x || !y) return 0;
	return Qstr(str_concat(x,y));
	}
	}

/* (length x) is the length of string x */
value type_length(value f)
	{
	if (!f->L) return 0;
	{
	string x = atom(type_str,arg(&f->R));
	if (!x) return 0;
	return Qnum(num_new_ulong(x->len));
	}
	}

/* (slice str pos len) returns the len bytes of str starting at pos, or 0 bytes
if pos or len exceeds the bounds of str. */
value type_slice(value f)
	{
	if (!f->L || !f->L->L || !f->L->L->L) return 0;
	{
	string x = atom(type_str,arg(&f->L->L->R));
	number y = atom(type_num,arg(&f->L->R));
	number z = atom(type_num,arg(&f->R));
	if (!x || !y || !z) return 0;
	{
	long pos = (long)*y;
	long len = (long)*z;

	if (pos < 0 || len < 0
		|| pos >= x->len
		|| len > x->len
		|| pos > x->len - len
		)
		{
		pos = 0;
		len = 0;
		}

	return Qstr(str_new_data(x->data + pos,len));
	}
	}
	}

value type_str_num(value f)
	{
	if (!f->L) return 0;
	{
	string x = atom(type_str,arg(&f->R));
	if (!x) return 0;
	return Qnum(str_num(x->data));
	}
	}

value type_is_str(value f)
	{
	if (!f->L) return 0;
	{
	string x = atom(type_str,arg(&f->R));
	return x ? C : F;
	}
	}
