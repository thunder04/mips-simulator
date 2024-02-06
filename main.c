// Multi-cycle MIPS Processor Simulator

#include "macros.h"
#include "multiplexers.h"
#include "special_function_registers.h"
#include "units/arithmetic_and_logic_shift_unit.h"
#include "units/arithmetic_and_logic_unit.h"
#include "units/register_file.h"
#include "units/memory.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Instruction Counter and μPC
static unsigned int IC = 0, microPC = 0;
// CPU Clock
static unsigned int clock = 0;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <object-file-path>\n", argv[0]);
    return 1;
  }

  FILE *fptr = fopen(argv[1], "r");

  if (fptr == NULL) {
    printf("Failed to read the object file: %s\n", strerror(errno));
    return 2;
  }

  // fread(fptr, 32,)

  fclose(fptr);
  return 0;
}
