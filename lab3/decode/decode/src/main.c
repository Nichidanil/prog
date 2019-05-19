#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    system("clear");

    if (argc != 4) {
        printf("Usage:\n");
        printf("coder encode <in-file-name> <out-file-name>\n");
        printf("coder decode <in-file-name> <out-file-name>\n");
        return -1;
    }
    if (strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0) {
        printf("Usage:\n");
        printf("coder encode <in-file-name> <out-file-name>\n");
        printf("coder decode <in-file-name> <out-file-name>\n");
        return -1;
    }
    if (strcmp(argv[1], "encode\0") == 0) {
        encode_file(argv[2], argv[3]);
    }
    if (strcmp(argv[1], "decode") == 0) {
        decode_file(argv[2], argv[3]);
    }

    return 0;
}