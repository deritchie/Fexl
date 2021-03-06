#include <value.h>
#include <basic.h>
#include <num.h>
#include <type_math.h>
#include <type_num.h>

static value op_num(value f, number op(number))
	{
	if (!f->L) return 0;
	{
	value x = eval(hold(f->R));
	if (x->T == type_num)
		replace_num(f, op(data(x)));
	else
		replace_void(f);
	drop(x);
	return 0;
	}
	}

static value op_num_num(value f, number op(number,number))
	{
	if (!f->L || !f->L->L) return 0;
	{
	value x = eval(hold(f->L->R));
	value y = eval(hold(f->R));
	if (x->T == type_num && y->T == type_num)
		replace_num(f, op(data(x),data(y)));
	else
		replace_void(f);
	drop(x);
	drop(y);
	return 0;
	}
	}

value type_add(value f) { return op_num_num(f,num_add); }
value type_sub(value f) { return op_num_num(f,num_sub); }
value type_mul(value f) { return op_num_num(f,num_mul); }
value type_div(value f) { return op_num_num(f,num_div); }
value type_pow(value f) { return op_num_num(f,num_pow); }
value type_round(value f) { return op_num(f,num_round); }
value type_trunc(value f) { return op_num(f,num_trunc); }
value type_abs(value f) { return op_num(f,num_abs); }
value type_sqrt(value f) { return op_num(f,num_sqrt); }
value type_exp(value f) { return op_num(f,num_exp); }
value type_log(value f) { return op_num(f,num_log); }
value type_sin(value f) { return op_num(f,num_sin); }
value type_cos(value f) { return op_num(f,num_cos); }
