#include "string.h"

size_t strlen(const char *s) {
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char *a, const char *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i] || a[i] == '\0' || b[i] == '\0')
            return (unsigned char)a[i] - (unsigned char)b[i];
    }
    return 0;
}

char *strcpy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
    char *d = dest;
    size_t i = 0;
    for (; i < n && src[i]; i++) d[i] = src[i];
    for (; i < n; i++) d[i] = '\0';
    return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    for (size_t i = 0; i < n; i++) p[i] = (unsigned char)c;
    return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
    return dest;
}

int memcmp(const void *a, const void *b, size_t n) {
    const unsigned char *pa = (const unsigned char *)a;
    const unsigned char *pb = (const unsigned char *)b;
    for (size_t i = 0; i < n; i++) {
        if (pa[i] != pb[i]) return pa[i] - pb[i];
    }
    return 0;
}

char *itoa(int value, char *str) {
    char buf[12];
    int i = 0, sign = 0;
    if (value == 0) { str[0] = '0'; str[1] = '\0'; return str; }
    if (value < 0) { sign = 1; value = -value; }
    while (value > 0 && i < (int)sizeof(buf)-1) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }
    if (sign) buf[i++] = '-';
    int j = 0;
    while (i > 0) str[j++] = buf[--i];
    str[j] = '\0';
    return str;
}
