#include "string.h"
#include <stdint.h>
#include "module.h"
#include "mem.h"

MODULE("STRING");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

int strlen(char* str) {
    int i = 0;
    while(*(str+i) != '\0')
        i++;
    return i;
}


void atoi(char *str, int* a)
{
	int k = 0;
	while(*str)
	{
		k = (k<<3)+(k<<1)+(*str)-'0';
		str++;
	}
	*a = k;
}

size_t strcrl(char* str, const char what, const char with)
{
	size_t i = 0;
	while(str[i] != 0)
	{
		if(str[i] == what) str[i] = with;
		i++;
	}
	return i;
}

size_t strcount(char* str, char c)
{
	size_t i = 0;
	while(*str--)
		if(*str == c) i++;
	return i;
}

size_t str_backspace(char* str, char c)
{
	size_t i = strlen(str);
	i--;
	while(i)
	{
		i--;
		if(str[i] == c)
		{
			str[i+1] = 0;
			return 1;
		}
	}
	return 0;
}

size_t strsplit(char* str, char delim)
{
	size_t n = 0;
	uint32_t i = 0;
	while(str[i])
	{
		if(str[i] == delim)
		{
			str[i] = 0;
			n++;
		}
		i++;
	}
	n++;
	return n;
}

size_t str_begins_with(const char* str, const char* with)
{
	size_t j = strlen(with);
	size_t i = 0;
	size_t ret = 1;
	while(with[j] != 0)
	{
		if(str[i] != with[i]) { ret = 0; break; }
		j--;
		i++;
	}
	return ret;
}

size_t strcmp(const char* str1, const char* str2)
{
	size_t res=0;
	while (!(res = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2)
		++str1, ++str2;

	return res;
}

char *strchrnul (const char *s, int c_in)
{
	const unsigned char *char_ptr;
	const unsigned long int *longword_ptr;
	unsigned long int longword, magic_bits, charmask;
	unsigned char c;
	c = (unsigned char) c_in;
	/* Handle the first few characters by reading one character at a time.
		Do this until CHAR_PTR is aligned on a longword boundary.  */
	for (char_ptr = (const unsigned char *) s;
		((unsigned long int) char_ptr & (sizeof (longword) - 1)) != 0;
		++char_ptr)
		if (*char_ptr == c || *char_ptr == '\0')
		return (void *) char_ptr;
	/* All these elucidatory comments refer to 4-byte longwords,
		but the theory applies equally well to 8-byte longwords.  */
	longword_ptr = (unsigned long int *) char_ptr;
	/* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
		the "holes."  Note that there is a hole just to the left of
		each byte, with an extra at the end:
		bits:  01111110 11111110 11111110 11111111
		bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
		The 1-bits make sure that carries propagate to the next 0-bit.
		The 0-bits provide holes for carries to fall into.  */
	magic_bits = -1;
	magic_bits = magic_bits / 0xff * 0xfe << 1 >> 1 | 1;
	/* Set up a longword, each of whose bytes is C.  */
	charmask = c | (c << 8);
	charmask |= charmask << 16;
	if (sizeof (longword) > 4)
		/* Do the shift in two steps to avoid a warning if long has 32 bits.  */
		charmask |= (charmask << 16) << 16;
	if (sizeof (longword) > 8)
		abort ();
	/* Instead of the traditional loop which tests each character,
		we will test a longword at a time.  The tricky part is testing
		if *any of the four* bytes in the longword in question are zero.  */
	for (;;)
		{
		/* We tentatively exit the loop if adding MAGIC_BITS to
			LONGWORD fails to change any of the hole bits of LONGWORD.
			1) Is this safe?  Will it catch all the zero bytes?
			Suppose there is a byte with all zeros.  Any carry bits
			propagating from its left will fall into the hole at its
			least significant bit and stop.  Since there will be no
			carry from its most significant bit, the LSB of the
			byte to the left will be unchanged, and the zero will be
			detected.
			2) Is this worthwhile?  Will it ignore everything except
			zero bytes?  Suppose every byte of LONGWORD has a bit set
			somewhere.  There will be a carry into bit 8.  If bit 8
			is set, this will carry into bit 16.  If bit 8 is clear,
			one of bits 9-15 must be set, so there will be a carry
			into bit 16.  Similarly, there will be a carry into bit
			24.  If one of bits 24-30 is set, there will be a carry
			into bit 31, so all of the hole bits will be changed.
			The one misfire occurs when bits 24-30 are clear and bit
			31 is set; in this case, the hole at bit 31 is not
			changed.  If we had access to the processor carry flag,
			we could close this loophole by putting the fourth hole
			at bit 32!
			So it ignores everything except 128's, when they're aligned
			properly.
			3) But wait!  Aren't we looking for C as well as zero?
			Good point.  So what we do is XOR LONGWORD with a longword,
			each of whose bytes is C.  This turns each byte that is C
			into a zero.  */
		longword = *longword_ptr++;
		/* Add MAGIC_BITS to LONGWORD.  */
		if ((((longword + magic_bits)
				/* Set those bits that were unchanged by the addition.  */
				^ ~longword)
			/* Look at only the hole bits.  If any of the hole bits
				are unchanged, most likely one of the bytes was a
				zero.  */
			& ~magic_bits) != 0
			/* That caught zeroes.  Now test for C.  */
			|| ((((longword ^ charmask) + magic_bits) ^ ~(longword ^ charmask))
				& ~magic_bits) != 0)
			{
			/* Which of the bytes was C or zero?
				If none of them were, it was a misfire; continue the search.  */
			const unsigned char *cp = (const unsigned char *) (longword_ptr - 1);
			if (*cp == c || *cp == '\0')
				return (char *) cp;
			if (*++cp == c || *cp == '\0')
				return (char *) cp;
			if (*++cp == c || *cp == '\0')
				return (char *) cp;
			if (*++cp == c || *cp == '\0')
				return (char *) cp;
			if (sizeof (longword) > 4)
				{
				if (*++cp == c || *cp == '\0')
					return (char *) cp;
				if (*++cp == c || *cp == '\0')
					return (char *) cp;
				if (*++cp == c || *cp == '\0')
					return (char *) cp;
				if (*++cp == c || *cp == '\0')
					return (char *) cp;
				}
			}
		}
	/* This should never happen.  */
	return NULL;
}

size_t strcspn(const char *s1, const char *s2)
{
	const char *p, *spanp;
	char c, sc;
	for (p = s1;;) {
		c = *p++;
		spanp = s2;
		do {
			if ((sc = *spanp++) == c)
				return (p - 1 - s1);
		} while (sc != 0);
	}
	/* NOTREACHED */
}

size_t strspn(const char *s1, const char *s2)
{
	const char *p = s1, *spanp;
	char c, sc;
cont:
	c = *p++;
	for (spanp = s2; (sc = *spanp++) != 0;)
		if (sc == c)
			goto cont;
	return (p - 1 - s1);
}

static char *oldss[128];
int oldsel = 0;
// glibc functions
char *__strtok_r (char *s, const char *delim, char **save_ptr)
{
	char *end;

	if (s == NULL)
		s = *save_ptr;

	if (*s == '\0')
		{
		*save_ptr = s;
		free(save_ptr);
		oldsel--;
		return NULL;
		}

	/* Scan leading delimiters.  */
	s += strspn (s, delim);
	if (*s == '\0')
		{
		*save_ptr = s;
		free(save_ptr);
		oldsel--;
		return NULL;
		}

	/* Find the end of the token.  */
	end = s + strcspn (s, delim);
	if (*end == '\0')
		{
		*save_ptr = end;
		return s;
		}

	/* Terminate the token and make *SAVE_PTR point past it.  */
	*end = '\0';
	*save_ptr = end + 1;
	return s;
}

char *strtok (char *s, const char *delim)
{
	char *olds = oldss[oldsel++];
	olds = malloc(512);
	return __strtok_r (s, delim, &olds);
}

char tbuf[32];
char bchars[] = "0123456789abcdefghijkmnlopqrstuvwxyzABCDEFGHIJKMNLOPQRSTUVWXYZ!@#$%^&*()";
void itoa(unsigned i,unsigned base,char* buf) {
    int pos = 0;
    int opos = 0;
    int top = 0;

    if (i == 0 || base > 72) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (i != 0) {
        tbuf[pos] = bchars[i % base];
        pos++;
        i /= base;
    }
    top=pos--;
    for (opos=0; opos<top; pos--,opos++)
        buf[opos] = tbuf[pos];
    buf[opos] = 0;
}

void itoa_s(int i,unsigned base,char* buf) {
    if (base > 16) return;
    if (i < 0) {
        *buf++ = '-';
        i *= -1;
    }
    itoa(i,base,buf);
}