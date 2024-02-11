opcodes = {
    0b000000: "add",
    0b000001: "sub",
    0b000010: "or",
    0b000011: "and",
    0b001000: "addi",
    0b001010: "ori",
    0b001011: "andi",
    0b100000: "lw",
    0b100001: "sw",
    0b010000: "beq",
    0b010001: "bne",
    0b010100: "j",
    0b010101: "jr",
    0b010110: "jal",
    0b010111: "jalr",
    0b110000: "addm",
    0b110100: "addm",
    0b110001: "subm",
    0b110101: "subm",
    0b110010: "orm",
    0b110110: "orm",
    0b110011: "andm",
    0b110111: "andm",
    0b111100: "addmi",
    0b111110: "ormi",
    # Keep a close eye on this one! The script didn't print it
    0b111111: "andmi",
    0b001100: "sll",
    0b001101: "srl",
    0b001110: "sra",
    0b001111: "ror",
    0b000100: "sllv",
    0b000101: "srlv",
    0b000110: "srav",
    0b000111: "rorv",
    0b011000: "slladd",
    0b011001: "srladd",
    0b011010: "sraadd",
    0b011011: "roradd",
    0b011100: "sllsub",
    0b011101: "srlsub",
    0b011110: "srasub",
    0b011111: "rorsub",
    0b010011: "break",
}

max = -1
for k in opcodes:
    if k > max:
        max = k
print(f"static int OPCODE_JUMP_TABLE[{max}] = {{")

for i in range(0, max):
    if i in opcodes:
        print(f"\t-1, // INSTRUCTION: 0b{i:06b} => {opcodes[i]}")
    else:
        print("\t-1, // NOTHING")
print("};")
