#include <stdio.h>
#include <string.h>
#include "die.h"
#include "str.h"

#include "value.h"
#include "double.h"
#include "fexl.h"
#include "form.h"
#include "lib.h"
#include "long.h"
#include "parse.h"
#include "qfile.h"
#include "qstr.h"
#include "resolve.h"

static void report_undef(value sym)
	{
	char *name = as_str(sym->L)->data;
	long line = get_long(sym->R);

	warn("Undefined symbol %s on line %ld%s%s", name, line,
		source_name[0] ? " of " : "",
		source_name
		);
	}

value type_argv(value f) /*TODO move to lib */
	{
	if (!f->L) return f;
	value x = eval(f->R);
	long i = get_long(x);
	value z = Qstr0(i >= 0 && i < argc ? argv[i] : "");
	drop(x);
	return z;
	}

/* This is the core context needed to bootstrap a larger context in Fexl. */
static value context(char *name)
	{
	if (strcmp(name,"dlopen") == 0) return Q(type_dlopen);
	if (strcmp(name,"dlsym") == 0) return Q(type_dlsym);
	if (strcmp(name,"Q") == 0) return Q(type_Q);
	if (strcmp(name,"source_file") == 0) return Qfile(source_fh);
	if (strcmp(name,"source_name") == 0) return Qstr0(source_name);
	if (strcmp(name,"source_line") == 0) return Qlong(source_line);
	if (strcmp(name,"base_path") == 0) return Q(type_base_path);

	if (strcmp(name,"argc") == 0) return Qlong(argc); /*TODO in Fexl */
	if (strcmp(name,"argv") == 0) return Q(type_argv); /*TODO in Fexl */

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

	return 0;
	}

/* Resolve all symbols in form f with the core context, reporting any undefined
symbols to stderr. */
value resolve(value f)
	{
	hold(f);
	value sym = first_symbol(f);
	if (sym == 0) return f;

	value def = 0;

	value content = sym->L;
	if (content->T == type_name)
		{
		char *name = as_str(content)->data;
		def = context(name);
		if (!def)
			{
			report_undef(sym);
			def = sym;
			}
		}
	else
		def = content; /* string literal */

	value form = abstract(sym,f);
	value resolved = resolve(form);
	value result = apply(resolved,def);

	hold(result);
	drop(form);
	drop(resolved);
	drop(f);
	return result;
	}
