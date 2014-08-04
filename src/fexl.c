#include <num.h>
#include <str.h>
#include <value.h>
#include <basic.h>
#include <convert.h>
#include <input.h>
#include <output.h>
#include <parse.h>
#include <parse_file.h>
#include <parse_string.h>
#include <string.h> /* strcmp */
#include <type_cmp.h>
#include <type_input.h>
#include <type_math.h>
#include <type_num.h>
#include <type_output.h>
#include <type_str.h>
#include <type_sym.h>

static void put_loc(unsigned long line)
	{
	if (line) { put(" on line "); put_ulong(line); }
	nl();
	}

static const char *curr_name;

static int match(const char *other)
	{
	return strcmp(curr_name,other) == 0;
	}

value type_get_from_file(value f) /*TODO*/
	{
	if (!f->L || !f->L->L) return f;
	return f;
	}

extern value type_eval_file(value f);

static value context(const char *name, unsigned long line)
	{
	{
	int ok;
	number n = str0_num(name,&ok);
	if (ok) return Qnum(n);
	}

	curr_name = name;
	if (match("say")) return Q(type_say);
	if (match("put")) return Q(type_put);
	if (match("nl")) return Q(type_nl);
	if (match("@")) return hold(Y);
	if (match("+")) return Q(type_add);
	if (match("-")) return Q(type_sub);
	if (match("*")) return Q(type_mul);
	if (match("/")) return Q(type_div);
	if (match("sqrt")) return Q(type_sqrt);
	if (match("get")) return Q(type_get);
	if (match("eq")) return Q(type_eq);
	if (match("put_to_error")) return Q(type_put_to_error);
	if (match("get_from_file")) return Q(type_get_from_file);
	if (match("slice")) return Q(type_slice);
	if (match("is_num")) return Q(type_is_num);
	if (match("is_str")) return Q(type_is_str);
	if (match("str_num")) return Q(type_str_num);
	if (match("num_str")) return Q(type_num_str);
	if (match(".")) return Q(type_concat);
	if (match("length")) return Q(type_length);
	if (match("T")) return hold(C);
	if (match("F")) return hold(F);
	if (match("I")) return hold(I);
	if (match("later")) return Q(type_later);
	if (match("once")) return Q(type_once);
	if (match("catch")) return Q(type_catch);
	/*TODO get_from_file */
	/*TODO get_from_input */
	/*TODO get_from_string */
	/*TODO get_from_source */
	if (match("lt")) return Q(type_lt);
	if (match("le")) return Q(type_le);
	if (match("eq")) return Q(type_eq);
	if (match("ne")) return Q(type_ne);
	if (match("ge")) return Q(type_ge);
	if (match("gt")) return Q(type_gt);
	if (match("parse_string")) return Q(type_parse_string);
	if (match("parse_file")) return Q(type_parse_file);
	if (match("eval_file")) return Q(type_eval_file);

	put_to_error();
	error_code = "Undefined symbol ";
	put(error_code); put(name); put_loc(line);
	return 0;
	}

/*TODO note that this works and is cool:
~END
get \ch
put "ch = " put ch nl
\\X
END

Can we make a version of that which reads *only* from the source file, whether
it's stdin or not?
*/

static value parse_file(const char *name)
	{
	value exp;
	input save_getd = getd;
	get_from_file(name);
	exp = parse_source();
	/*TODO we might want to keep a handle to the tail of the source file. */
	getd = save_getd;
	return exp;
	}

static value read_program(const char *name)
	{
	value exp = parse_file(name);
	if (exp)
		return resolve(exp,context);
	else
		{
		if (error_code)
			{
			put_to_error();
			put(error_code); put_loc(error_line);
			}
		return 0;
		}
	}

value type_eval_file(value f)
	{
	if (!f->L) return f;
	{
	value g = 0;
	value x = eval(hold(f->R));
	if (is_atom(type_str,x))
		{
		const char *name = ((string)x->R)->data;
		g = eval(read_program(name));
		}
	drop(x);
	return g;
	}
	}

int main(int argc, char *argv[])
	{
	beg_basic();
	{
	const char *name = argc > 1 ? argv[1] : "";
	value f = eval(read_program(name));
	if (!f && !error_code)
		{
		error_code = "The program used a data type incorrectly.";
		put_to_error();
		put(error_code); nl();
		}
	drop(f);
	}
	end_basic();
	end_value();
	return (error_code ? 1 : 0);
	}
