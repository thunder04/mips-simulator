# A script to add the current PC to the end of
# comments as if there are no jump instructions

import sys, os

if len(sys.argv) < 2:
    print(f"Usage: {sys.argv[0]} <object-file>")
    exit(1)

PC = 0x00400000

with open(sys.argv[1], 'r+') as file:
    lines = file.readlines()
    did_see_comment = False

    for index, line in enumerate(lines):
        if line.startswith('#'):
            did_see_comment = True
        elif line.startswith('0') or line.startswith('1'):
            if did_see_comment:
                lines[index - 1] = lines[index - 1][:-(len(os.linesep))] + f"\t(PC = 0x{PC:08x}){os.linesep}"
                print(f"PC = 0x{PC:08x} => {PC:026b}")

            did_see_comment =False
            PC += 4

    file.seek(0)
    file.writelines(lines)
