extern value C;
extern value S;
extern value I;
extern value R;
extern value L;
extern value Qitem;
extern value Qquery;
extern value type_C(value f); /*TODO*/
extern value replace_boolean(value f, int flag);
extern value replace_maybe(value f, value x);
extern value op_is_atom(value f, type t);
extern int is_item(value f);
extern value item(value f, value g);
extern value yield(value f, value g);
extern value op_compare(value f, type t, int cmp(value,value));
extern value resolve_basic(const char *name);
extern void beg_basic(void);
extern void end_basic(void);
