#include <stdio.h>
#include <value.h>
#include <basic.h>
#include <define.h>
#include <die.h>
#include <fexl.h>
#include <file.h>
#include <show.h>
#include <str.h>
#include <string.h>
#include <system.h>
#include <type_double.h>
#include <type_file.h>
#include <type_long.h>
#include <type_str.h>
#include <type_sym.h>
#include <type_var.h>

static const char *curr_name;

static int match(const char *name)
	{
	return strcmp(curr_name,name) == 0;
	}

static int starts(const char *prefix)
	{
	return strncmp(curr_name,prefix,strlen(prefix)) == 0;
	}

static value define_name(const char *name)
	{
	curr_name = name;

	/* Integer number (long) */
	{
	long num;
	if (string_long(name,&num)) return Qlong(num);
	}

    /* Floating point number (double) */
	{
	double num;
	if (string_double(name,&num)) return Qdouble(num);
	}

	if (match("say")) return Q(type_say);
	if (match("print")) return Q(type_print);
	if (match("nl")) return Q(type_nl);
	if (match("@")) return Q(type_Y);
	if (match("later")) return Q(type_later);
	if (match("T")) return C;
	if (match("F")) return Q(type_F);
	if (match("pair")) return Qpair;
	if (match("cons")) return Qcons;

	/*TODO do full versions of arithmetic operators, with long/double */
	if (match("+")) return Q(type_long_add);
	if (match("-")) return Q(type_long_sub);
	if (match("*")) return Q(type_long_mul);
	if (match("/")) return Q(type_long_div);

	if (match("I")) return I;
	if (match("?")) return query;
	if (match("halt")) return Q(type_halt);

	if (starts("long_"))
		{
		if (match("long_add")) return Q(type_long_add);
		if (match("long_sub")) return Q(type_long_sub);
		if (match("long_mul")) return Q(type_long_mul);
		if (match("long_div")) return Q(type_long_div);
		if (match("long_cmp")) return Q(type_long_cmp);
		if (match("long_string")) return Q(type_long_string);
		if (match("long_char")) return Q(type_long_char);
		return 0;
		}

	if (starts("double_"))
		{
		if (match("double_add")) return Q(type_double_add);
		if (match("double_sub")) return Q(type_double_sub);
		if (match("double_mul")) return Q(type_double_mul);
		if (match("double_div")) return Q(type_double_div);
		if (match("double_cmp")) return Q(type_double_cmp);
		if (match("double_string")) return Q(type_double_string);
		if (match("double_long")) return Q(type_double_long);
		return 0;
		}

	if (match("order")) return Q(type_order);
	if (match("show")) return Q(type_show);
	if (match("source_label")) return Qstr0(source_label);
	if (match("source_line")) return Qlong(source_line);

	if (match("C")) return C;
	if (match("S")) return S;
	if (match("R")) return R;
	if (match("L")) return L;
	if (match("Y")) return Q(type_Y);

	if (match("yes")) return Q(type_yes);
	if (match("no")) return C;
	if (match("pi")) return Qdouble(3.14159265358979);

	if (match("new")) return Q(type_new);
	if (match("set")) return Q(type_set);
	if (match("get")) return Q(type_get);

	if (match("argc")) return Qlong(argc);
	if (match("argv")) return Q(type_argv);
	/* TODO env */

	if (match("stdin")) return Qfile(stdin);
	if (match("stdout")) return Qfile(stdout);
	if (match("stderr")) return Qfile(stderr);

	if (match("base_path")) return Qstr(base_path());
	return 0;
	}

value define_sym(value x)
	{
	struct sym *sym = get_sym(x);
	if (sym->line < 0)
		return sym->name; /* literal string */

	const char *name = get_str(sym->name)->data;

	value def = define_name(name);
	if (def) return def;

	warn("Undefined symbol %s on line %d%s%s", name, sym->line,
		source_label[0] ? " of " : "",
		source_label);
	return x;
	}
