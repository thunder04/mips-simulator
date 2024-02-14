#ifndef _UTILS
#define _UTILS

// buf should be at least `sizeof(unsigned int) * 8 + 1` long. \0 is
// automatically added at the end
void format_bits(char* buf, unsigned int num);

#endif
