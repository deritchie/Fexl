#include <string.h>
#include "memory.h"
#include "str.h"

/* Create a string capable of holding len bytes plus trailing NUL. */
struct str *str_new(long len)
	{
	if (len < 0) len = 0;
	struct str *x = new_memory(sizeof(struct str) + len + 1);
	x->len = len;
	return x;
	}

struct str *str_new_data(const char *data, long len)
	{
	struct str *x = str_new(len);
	memcpy(x->data, data, len);
	x->data[len] = '\000';
	return x;
	}

struct str *str_new_data0(const char *data)
	{
	return str_new_data(data, strlen(data));
	}

void str_free(struct str *x)
	{
	free_memory(x, sizeof(struct str) + x->len + 1);
	}

/* Return byte at pos, or 0 if out of bounds. */
long str_at(struct str *str, long pos)
	{
	return pos >= 0 && pos < str->len ? str->data[pos] : 0;
	}

/* Append x and y. */
struct str *str_append(struct str *x, struct str *y)
	{
	struct str *z = str_new(x->len + y->len);
	memcpy(z->data, x->data, x->len);
	memcpy(z->data + x->len, y->data, y->len);
	z->data[z->len] = '\000'; /* add trailing NUL byte */
	return z;
	}

/* Compare strings x and y, returning negative if x < y, zero if x == y, or
positive if x > y. */
int str_cmp(struct str *x, struct str *y)
	{
	if (x == y) return 0;
	long min_len = x->len < y->len ? x->len : y->len;
	int cmp = memcmp(x->data, y->data, min_len);

	if (cmp == 0 && x->len != y->len)
		cmp = x->len < y->len ? -1 : 1;

	return cmp;
	}
