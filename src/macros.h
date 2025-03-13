#pragma once

// General macros and global constants go here.

// Comment out the next line to disable DEBUG messages
#define DEBUG

// Used to print to stdout/stderr with colors
#define ANSI_FR "\x1b[31m"
#define ANSI_FG "\x1b[32m"
#define ANSI_FC "\x1b[36m"
#define ANSI_FM "\x1b[35m"
#define ANSI_0 "\x1b[0m"

// A helper macro to create a mask of n bits. For example: n = 5 => 0b11111
#define MASK(n) ((1 << (n)) - 1)

// A helper macro that implements the sign extend function for the immediate
// field (of 16 bits) of IR.
// Explanation: If the 16th bit (15 if counting from 0) (the sign) is 1, then
// fill the rest of the integer with 1s by ORing a mask of 16 bits, moved to
// the MSB with imm itself. Otherwise, leave it as it is.
#define IMM_SIGN_EXTEND(imm)                                                   \
    (((imm) >> 15) == 1 ? (MASK(16) << 16) | (imm) : (imm))

#ifdef DEBUG
#define DEBUG_PRINTF(format, ...)                                              \
    printf(ANSI_FM "[DEBUG] " ANSI_0 format, ##__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)                                                      \
    do {                                                                       \
    } while (0)
#endif
