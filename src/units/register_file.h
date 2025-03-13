#pragma once

#include <stdio.h>

struct RegOut {
    int ReadData1, ReadData2;
};

struct RegOut reg(unsigned int ReadRegister1, unsigned int ReadRegister2,
                  unsigned int WriteRegister, unsigned int WriteData,
                  unsigned int RegWrite);

void write_reg_stats_section(FILE *fstatsptr);
