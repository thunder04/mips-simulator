# mips-simulator

A minimal implementation of a Multi-cycle MIPS Simulator. It is not intended to be used for production use.

It accepts a pseudo-object file, which is a file consisting of multiple 32-bit words, separated by a newline.
The program assumes the file is valid. Comments may be defined in a line starting with `#`. No empty lines are allowed.

## Building the simulator

Run `clang -o mips-simulator src/**/*.c`
