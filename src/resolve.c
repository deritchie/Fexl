#include <dlfcn.h>
#include <stdio.h>
#include "die.h"
#include "str.h"

#include "value.h"
#include "basic.h"
#include "double.h"
#include "form.h"
#include "long.h"
#include "parse.h"
#include "qstr.h"
#include "resolve.h"

static void *find_symbol(const char *prefix0, struct str *name)
	{
	struct str *prefix = str_new_data0(prefix0);
	struct str *full_name = str_append(prefix, name);

	void *def = dlsym(NULL, full_name->data);

	str_free(prefix);
	str_free(full_name);

	return def;
	}

/* This is the core context (environment) for Fexl. */
static value define_name(struct str *name)
	{
	/* Integer number (long) */
	{
	long num;
	if (string_long(name->data,&num)) return Qlong(num);
	}

    /* Floating point number (double) */
	{
	double num;
	if (string_double(name->data,&num)) return Qdouble(num);
	}

	/* Look up type_<name>. */
	{
	value (*fn)(value) = find_symbol("type_",name);
	if (fn) return Q(fn);
	}

	/* Look up const_<name>. */
	{
	value (*fn)(void) = find_symbol("const_",name);
	if (fn) return fn();
	}

	return 0;
	}

/* (define_name name) Looks up name in the core context and returns (yes val)
or no. */
value type_define_name(value f)
	{
	if (!f->L) return f;
	value x = eval(f->R);
	value def = define_name(get_str(x));
	value result = maybe(def);
	drop(x);
	return result;
	}

static value case_no;
static value case_yes;
static value curr_define_string;
static value curr_define_name;

/* Resolve all symbols in the form with the core context, reporting any
undefined symbols to stderr. */
static value resolve(value form)
	{
	value sym = first_symbol(form);
	if (sym == 0) return form;

	value define = (sym->R->L->T == type_C)
		? curr_define_string : curr_define_name;
	value name = sym->L;

	value def = eval(A(A(A(define,name),case_no),case_yes));

	value fn;
	if (def->L == case_yes)
		fn = def->R;
	else
		{
		report_undef(sym);
		fn = sym;
		if (def != case_no)
			bad_type();
		}

	value new_form = abstract(sym,form);
	value resolved = resolve(new_form);
	value result = apply(resolved,fn);

	drop(def);
	drop(new_form);
	return result;
	}

/* (resolve define_string define_name form) */
value type_resolve(value f)
	{
	if (!f->L || !f->L->L || !f->L->L->L) return f;

	curr_define_string = eval(f->L->L->R);
	curr_define_name = eval(f->L->R);
	value form = eval(f->R);

	case_no = Qlong(1); hold(case_no);
	case_yes = Qlong(2); hold(case_yes);

	value result = resolve(form);

	drop(case_no);
	drop(case_yes);

	drop(curr_define_string);
	drop(curr_define_name);
	drop(form);

	return A(later,result);
	}

value resolve_standard(value form)
	{
	curr_define_string = eval(yes);
	curr_define_name = eval(Q(type_define_name));

	case_no = Qlong(1); hold(case_no);
	case_yes = Qlong(2); hold(case_yes);

	hold(form);
	value f = resolve(form);
	hold(f);
	drop(form);

	if (f->T == type_form)
		die(0); /* Form has undefined symbols. */

	drop(case_no);
	drop(case_yes);

	drop(curr_define_string);
	drop(curr_define_name);

	return f;
	}
