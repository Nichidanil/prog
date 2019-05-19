#include "command.h"
#include "coder.h"
#include <stdio.h>
#include <stdlib.h>

int encode_file(const char* in_file_name, const char* out_file_name)
{
    FILE* txt;
    FILE* bin;

    txt = fopen(in_file_name, "r");
    if (txt == NULL) {
        printf("Error opening file %s\n", in_file_name);
        return -1;
    }
    bin = fopen(out_file_name, "w+b");
    if (bin == NULL) {
        printf("Error opening file %s\n", out_file_name);
        return -1;
    }

    CodeUnit* code_unit = (CodeUnit*)malloc(sizeof(CodeUnit));
    uint32_t code_point;

    while (!feof(txt)) {
        fscanf(txt, "%" SCNx32, &code_point);
        if (encode(code_point, code_unit) == -1) {
            continue;
        }
        fwrite(code_unit->code, sizeof(uint8_t), code_unit->length, bin);
    }

    // printf("%" PRIx32, code_point);

    return 0;
}

int decode_file(const char* in_file_name, const char* out_file_name)
{
    FILE* bin;
    FILE* txt;

    bin = fopen(in_file_name, "rb");
    if (bin == NULL) {
        printf("Error opening file %s\n", out_file_name);
        return -1;
    }

    txt = fopen(out_file_name, "w");
    if (txt == NULL) {
        printf("Error opening file %s\n", in_file_name);
        return -1;
    }

    CodeUnit* code_unit = (CodeUnit*)malloc(sizeof(CodeUnit));
    uint32_t value;

    // fseek(bin, 6, SEEK_SET);
    while (!feof(bin)) {
        if (read_next_code_unit(bin, code_unit) == -1) {
            continue;
        }
        value = decode(code_unit);
        if (value == -1) {
            continue;
        }

        write_code_unit(txt, value);
    }
    return 0;
}
