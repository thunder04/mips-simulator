struct RegOut {
  int ReadData1, ReadData2;
};

struct RegOut reg(int ReadRegister1, int ReadRegister2, int WriteRegister,
                  int WriteData, int RegWrite);
