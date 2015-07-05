/*
 * Very simple 32-bit example disassembler program
 * (c) 2004    jt <at> klake.org
 * (c) 2015  mail <at> alexeevdv.ru
 */

// C++ standard includes

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// Third party includes
#include "libdasm.h"    // step 0: include library header


// disassembled data buffer
unsigned char data[] = "\x01\x02";

int main()
{
    // step 1: declare struct INSTRUCTION
    INSTRUCTION inst;
    char string[256];

    // step 2: fetch instruction
    get_instruction(&inst, data, MODE_32);

    // step 3: print it
    get_instruction_string(&inst, FORMAT_INTEL, 0, string, sizeof(string));
    printf("%s\n", string);

    return 0;
}

