typedef struct value *value;
typedef value (*type)(value);

struct value
	{
	unsigned long N;
	type T;
	value L;
	value R;
	};

extern void end_value(void);
extern value hold(value f);
extern void drop(value f);
extern value V(type T, value L, value R);
extern value Q(type T);
extern value D(type T, void *x);
extern value apply(value x, value y);
extern value type_A(value f);
extern value A(value x, value y);
extern value eval(value f);
