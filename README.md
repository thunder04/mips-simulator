# mips-simulator

A minimal implementation of a Multi-cycle MIPS Simulator. It is not intended to be used for production use.

It accepts a pseudo-object file, which is a file consisting of multiple 32-bit words, separated by a newline.
The program assumes the file is valid. Comments may be defined in a line starting with `#`.

## Building The Simulator

Run `make clean; make all`

> [!IMPORTANT]
> It has been tested only on Linux.

## Interesting Places

<sup>More like keypoints for the university exercise</sup>

- The `clock` of the CPU is stored at [`src/units/control/control.c`](src/units/control/control.c#L19)
- The `μPC` of the CPU is stored at [`src/units/control/control.c`](src/units/control/control.c#L19)
- The microcode for the instructions is stored at [`src/units/control/microcode.c`](src/units/control/microcode.c#L11)
- Registers, memory and Special Function Registers are stored at:
  - For registers, [`src/units/register_file.c`](src/units/register_file.c)
  - For memory, [`src/units/memory.c`](src/units/memory.c)
  - For SFRs, [`src/special_function_registers.c`](src/special_function_registers.c)
- The ALSU is stored at [`src/units/arithmetic_and_logic_shift_unit.c`](src/units/arithmetic_and_logic_shift_unit.c)
