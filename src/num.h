typedef double *number;
extern void num_free(number x);
extern number num_new_double(double val);
extern number num_new_ulong(unsigned long val);
extern number num_add(number x, number y);
extern number num_sub(number x, number y);
extern number num_mul(number x, number y);
extern number num_div(number x, number y);
extern number num_pow(number x, number y);
extern number num_round(number x);
extern number num_trunc(number x);
extern number num_abs(number x);
extern number num_sqrt(number x);
extern int num_cmp(number x, number y);
extern void put_num(number x);
