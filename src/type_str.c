#include <num.h>
#include <str.h>
#include <value.h>
#include <basic.h>
#include <convert.h>
#include <type_num.h>
#include <type_str.h>

value type_str(value f)
	{
	if (!f->L) return 0;
	replace_void(f);
	return 0;
	}

value Qstr(string x)
	{
	return D(type_str,x,(type)str_free);
	}

void replace_str(value f, string x)
	{
	replace_D(f,type_str,x,(type)str_free);
	}

/* (. x y) is the concatenation of strings x and y. */
value type_concat(value f)
	{
	if (!f->L || !f->L->L) return 0;
	{
	value x = eval(hold(f->L->R));
	value y = eval(hold(f->R));
	if (x->T == type_str && y->T == type_str)
		replace_str(f, str_concat(data(x),data(y)));
	else
		replace_void(f);
	drop(x);
	drop(y);
	return 0;
	}
	}

/* (length x) is the length of string x */
value type_length(value f)
	{
	if (!f->L) return 0;
	{
	value x = eval(hold(f->R));
	if (x->T == type_str)
		replace_num(f, num_new_ulong(((string)data(x))->len));
	else
		replace_void(f);
	drop(x);
	return 0;
	}
	}

/* LATER str_find, a.k.a. index */

/* (slice str pos len) returns the len bytes of str starting at pos, clipping
if necessary to stay within the bounds of str.  Returns void if pos or len is
negative. */
value type_slice(value f)
	{
	if (!f->L || !f->L->L || !f->L->L->L) return 0;
	{
	value x = eval(hold(f->L->L->R));
	value y = eval(hold(f->L->R));
	value z = eval(hold(f->R));
	if (x->T == type_str && y->T == type_num && z->T == type_num)
		{
		double yn = *((number)data(y));
		double zn = *((number)data(z));
		if (yn >= 0 && zn >= 0)
			{
			string xs = data(x);
			unsigned long pos = (unsigned long)yn;
			unsigned long len = (unsigned long)zn;

			if (pos > xs->len)
				pos = xs->len;

			if (len > xs->len - pos)
				len = xs->len - pos;

			replace_str(f, str_new_data(xs->data + pos,len));
			}
		else
			replace_void(f);
		}
	else
		replace_void(f);

	drop(x);
	drop(y);
	drop(z);
	return 0;
	}
	}

/* Convert string to number if possible. */
value type_str_num(value f)
	{
	if (!f->L) return 0;
	{
	value x = eval(hold(f->R));
	if (x->T == type_str)
		{
		number n = str0_num(((string)data(x))->data);
		if (n)
			replace_num(f,n);
		else
			replace_void(f);
		}
	else
		replace_void(f);
	drop(x);
	return 0;
	}
	}

value type_is_str(value f)
	{
	return op_is_type(f,type_str);
	}
