#include "string.h"
#include <stdint.h>

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