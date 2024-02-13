// Multi-cycle MIPS Processor Simulator

#include "macros.h"
#include "special_function_registers.h"
#include "units/control/control.h"
#include "units/memory.h"
#include "units/register_file.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// +2 to account for \0 and \n
#define LINE_LIMIT 32 + 2

void parse_file(FILE *fptr);

int main(int argc, char *argv[]) {
  // Initialize the $sp register
  reg(0, 0, 29, MEM_START_OF_SP, 1);

  if (argc < 2) {
    printf("Usage: %s <object-file-path>\n", argv[0]);
    return 1;
  }

  FILE *fptr = fopen(argv[1], "r");

  if (fptr == NULL) {
    printf("Failed to read the pseudo-object file: %s\n", strerror(errno));
    return 2;
  }

  // Parse and close the file as it's not needed anymore
  parse_file(fptr);
  fclose(fptr);

  printf("\n=== Starting simulation ===\n");

  // Run the Control unit, which, for simplicity reasons, will also use (call)
  // the rest of the units. Something like running an event loop
  control();

  printf("\n=== Ending simulation ===\n");

  return 0;
}

void parse_file(FILE *fptr) {
  // Get file's size
  fseek(fptr, 0, SEEK_END);
  long flen = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  if (flen > MEM_MAX_PROGRAM_SIZE) {
    printf("The file exceeds %d bytes\n", MEM_MAX_PROGRAM_SIZE);
    exit(4);
  }

  char buf[LINE_LIMIT];
  unsigned int addr = PC;

  while (fgets(buf, LINE_LIMIT, fptr)) {
    char *endptr;
    unsigned int word = (unsigned int)strtol(buf, &endptr, 2);

    if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r') {
      DEBUG_PRINTF("Skipping line \"%s\"\n", buf);

      continue;
    }

    mem(addr, 0, 1, word);
    addr += 4;
  }
}
