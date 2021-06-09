#ifndef STRING_H
#define STRING_H

#include <stddef.h>

int strlen(char* str);
size_t strcmp(const char* str1, const char* str2);

void atoi(char *str, int* a);

size_t strcrl(char* str, const char what, const char with);
size_t str_begins_with(const char* str, const char* with);
size_t str_backspace(char* str, char c);
size_t strcount(char* str, char c);
size_t strsplit(char* str, char delim);

void itoa(unsigned i,unsigned base,char* buf);
void itoa_s(int i,unsigned base,char* buf);

#endif