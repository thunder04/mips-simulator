// Multi-cycle MIPS Processor Simulator

#include "macros.h"
#include "special_function_registers.h"
#include "units/control/control.h"
#include "units/memory.h"
#include "units/register_file.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// +2 to account for \0 and \n
#define LINE_LIMIT 32 + 2

void parse_file(FILE *fptr);
void write_stats_file(FILE *fstatsptr);

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <object-file-path> <stats-file>\n", argv[0]);
    return 1;
  }

  FILE *fptr = fopen(argv[1], "r");
  FILE *fstatsptr = fopen(argv[2], "w");

  if (fptr == NULL) {
    printf("Failed to open the pseudo-object file for reading: %s\n",
           strerror(errno));
    return EXIT_FAILURE;
  }

  if (fstatsptr == NULL) {
    printf("Failed to open the stats file for writing: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  // Initialize the $sp register
  reg(0, 0, 29, MEM_START_OF_SP, 1);

  // Parse and close the file as it's not needed anymore
  parse_file(fptr);
  fclose(fptr);

  // Run the Control unit, which, for simplicity reasons, will also use (call)
  // the rest of the units. Something like running an event loop
  printf("\n=== Starting simulation ===\n");
  control();
  printf("\n=== Ending simulation ===\n");

  // Write to the stats file
  write_stats_file(fstatsptr);
  fclose(fstatsptr);

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

    // If string parsing didn't stop at the end
    if (*endptr != '\0' && *endptr != '\n' && *endptr != '\r' ||
        // Or the string was empty
        buf == endptr) {
      DEBUG_PRINTF("Skipping line \"%s\"\n", buf);

      continue;
    }

    mem(addr, 0, 1, word);
    addr += 4;
  }
}

void write_stats_file(FILE *fstatsptr) {
  fprintf(fstatsptr, "===== Stats =====\n");
  fprintf(fstatsptr, "CPI = %d\nclock = %d\n", 0, clock);

  write_reg_stats_section(fstatsptr);
  write_mem_stats_section(fstatsptr);
}
