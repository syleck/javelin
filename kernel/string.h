#ifndef STRING_H
#define STRING_H

#include <stddef.h>

/**
 * @brief Returns length of string.
 * 
 * @param str String in question.
 * @return int Length.
 */
int strlen(char* str);
/**
 * @brief Compares a string with another.
 * 
 * @param str1 String 1.
 * @param str2 String 2.
 * @return size_t Differential between the two strings. If equal to 0, that means they are the same.
 */
size_t strcmp(const char* str1, const char* str2);

/**
 * @brief Converts a string into an integer.
 * 
 * @param str String.
 * @param a Output integer.
 */
void atoi(char *str, int* a);

size_t strcrl(char* str, const char what, const char with);
size_t str_begins_with(const char* str, const char* with);
size_t str_backspace(char* str, char c);
size_t strcount(char* str, char c);
size_t strsplit(char* str, char delim);
char *strtok (char *s, const char *delim);

void itoa(unsigned i,unsigned base,char* buf);
void itoa_s(int i,unsigned base,char* buf);

#endif