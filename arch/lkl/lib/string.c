#include <asm/types.h>
#include <asm/host_ops.h>
#include <linux/types.h>
void *__memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;
	const char *s = src;

	/* from lib/string.c */
	if (lkl_ops->memcpy)
		return lkl_ops->memcpy(dest, src, count);

	while (count--)
		*tmp++ = *s++;
	return dest;
}

void *__memset(void *s, int c, size_t count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}
