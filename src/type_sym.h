typedef struct symbol *symbol;
struct symbol
	{
	value name;
	unsigned long line;
	};

extern value type_sym(value f);
extern value Qsym(string name, unsigned long line);
extern value app(value f, value g);
extern value Qsubst(value p, value f);
extern value lam(value sym, value exp);
extern value type_subst(value f);
extern value type_resolve(value f);
