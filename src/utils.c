#include <stdio.h>

// buf should be at least `sizeof(unsigned int) * 8 + 1` long. \0 is
// automatically added at the end
void format_bits(char *buf, unsigned int num) {
  int l = l = sizeof(num) * 8;

  for (int b = 0; b < l; ++b) {
    buf[b] = ((num >> (l - b - 1)) & 0x01) + '0';
  }

  buf[l] = '\0';
}
