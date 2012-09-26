extern void type_string(value);
extern value Qchars(const char *data, long len);
extern value Qcopy_chars(const char *data, long len);
extern value Qcopy_string(const char *data);
extern value Qstring(const char *data);
extern value Qstrcat(const char *x, const char *y);
extern char *string_data(value);
extern long string_len(value);
extern int string_eq(value, value);
extern int string_long(char *beg, long *num);
extern int string_double(char *beg, double *num);
