extern value type_C(value);
extern value type_S(value);
extern value type_I(value);
extern value type_F(value f);
extern value type_R(value f);
extern value type_L(value f);
extern value type_Y(value f);
extern value type_cons(value f);
extern value type_bad(value f);
extern value type_eval(value f);
extern value type_later(value f);
extern value type_catch(value f);
extern value Qboolean(int x);
extern value Qis_atom(type t, value f);
extern value C;
extern value S;
extern value I;
extern value F;
extern value R;
extern value L;
extern value Y;
extern value Qeval;
extern value Qcons;
extern void beg_basic(void);
extern void end_basic(void);
