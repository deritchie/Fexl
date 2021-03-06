typedef struct string *string;

struct string
	{
	unsigned long len;
	char data[1];
	};

extern string str_new(unsigned long len);
extern string str_new_data(const char *data, unsigned long len);
extern string str_new_data0(const char *data);
extern void str_free(string x);
extern string str_concat(string x, string y);
extern int str_cmp(string x, string y);
extern int str_eq(string x, string y);
extern void put_str(string x);
