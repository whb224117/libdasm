
/*
 * Simple example disassembler program
 * (c) 2004 - 2005    jt <at> klake.org
 * (c) 2015         mail <at> alexeevd.ru
 *
 */

// C standard includes
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>

// Disassembler includes

// Third party includes
#include "libdasm.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

unsigned char * read_file(int *, char *);

/*
 * Handler for segmentation faults
 * If this fires, there's probably bug in libdasm parsing code
 */
void sighandler(int sig)
{
    printf("*** SIGSEGV catched\n");
    printf("There is probably bug in libdasm parsing code");
    exit (1);
}

int main(int argc, char **argv)
{
    INSTRUCTION inst;   // declare struct INSTRUCTION
    unsigned char *data;
    int i, c = 0, bytes, format = FORMAT_INTEL, mode = MODE_32, size, len;
    char string[256];

    if (argc < 2)
    {
        printf("\nSimple x86 disassembler example\n");
        printf("Compiled with libdasm %d.%d.%d.%d\n\n",
            LIBDASM_VERSION_MAJOR,
            LIBDASM_VERSION_MINOR1,
            LIBDASM_VERSION_MINOR2,
            LIBDASM_VERSION_MINOR3);
        printf("Usage: %s <file> [-a|-i] [bytes]\n"
               "  file    file to be disassembled (required)\n"
               "  -a      format: ATT (optional)\n"
               "  -i      format: INTEL (optional, default)\n"
               "  bytes   show raw instruction data (optional, default 8)\n\n",
               argv[0]);
        exit (1);
    }
    data = read_file(&size, argv[1]);

    bytes = 8;
    if (argc > 2)
    {
        if (argv[2][0] == '-')
        {
            switch(argv[2][1])
            {
                case 'a':
                    format = FORMAT_ATT;
                    break;
                case 'i':
                    format = FORMAT_INTEL;
                    break;
            }
            if (argc > 3)
            {
                bytes = atoi(argv[3]);
            }
        }
        else
        {
            bytes = atoi(argv[2]);
        }
    } 

    signal(SIGSEGV, sighandler);

    while (c < size)
    {
        /*
         * get_instruction() has the following parameters:
         *
         * - &inst is a pointer to struct INSTRUCTION
         * - data + c is pointer to data to be disassembled
         * - disassemble in 32-bit mode: MODE_32 
         */
        len = get_instruction(&inst, data + c, mode);

        // Illegal opcode or opcode longer than remaining buffer
        if (!len || (len + c > size))
        {
            printf("%.8x  ", c);
            if (bytes)
            {
                printf("%.2x  ", data[c]);
                for (i = 1; i < bytes*2 - 1; i++)
                {
                    printf(" ");
                }
            }
            if (format == FORMAT_INTEL)
            {
                printf("db 0x%.2x\n", *(data + c));
            }
            else
            {
                printf(".byte 0x%.2x\n", *(data + c));
            }
            c++;
            continue;
        }

        /*
         * Print absolute offset and raw data bytes up to 'bytes'
         * (not needed, but looks nice).
         *
         */
        printf("%.8x  ", c);
        if (bytes)
        {
            for (i = 0; i < MIN(bytes, len); i++)
            {
                printf("%.2x", data[c + i]);
            }
            printf("  ");
            for (i = MIN(bytes, len); i < bytes*2 - len; i++)
            {
                printf(" ");
            }
        }
        /*
         * Print the parsed instruction, format using user-supplied
         * format. We could of course format the instruction in some
         * other way by accessing struct INSTRUCTION members directly.
         */
        get_instruction_string(&inst, format, (DWORD)c, string, sizeof(string));
        printf("%s\n", string);

        c += len;
    } 

    return 0;
}

/* Read file in buffer */

unsigned char * read_file(int *len, char *name)
{
    char        *buf;
    FILE        *fp;
    int         c;
    struct stat sstat;

    if ((fp = fopen(name, "r+b")) == NULL)
    {
        fprintf(stderr, "Error: unable to open file \"%s\"\n", name);
        exit(1);
    }

    /* Get file len */
    if ((c = stat(name, &sstat)) == -1)
    {
        fprintf(stderr, "Error: stat\n");
        exit (1);
    }
    *len = sstat.st_size;

    /* Allocate space for file */
    if (!(buf = (char *)malloc(*len)))
    {
        fprintf(stderr, "Error: malloc\n");
         exit (1);
    }

    /* Read file in allocated space */
    if ((c = fread(buf, 1, *len, fp)) != *len)
    {
        fprintf(stderr,"Error: fread\n");
        exit (1);
    }

    fclose(fp);

    return (buf);
}
