#include "coder.h"
#include <stdio.h>

int get_count_byte(uint32_t code_point, CodeUnit* code_unit)
{
    code_unit->length = 1;
    while (code_point >= 0x80) {
        code_point >>= 6;
        code_unit->length++;
        if (code_unit->length > 4) {
            return -1;
        }
    }
    return 0;
}

void choose_your_pattern(uint32_t code_point, CodeUnit* code_unit)
{
    switch (code_unit->length) {
    case 1:
        code_unit->code[0] = (code_point & 0x7f) | 0x00;
        break;
    case 2:
        code_unit->code[0] = ((code_point >> 6) & 0x1f) | 0xc0;
        code_unit->code[1] = (code_point & 0x3f) | 0x80;
        break;
    case 3:
        code_unit->code[0] = ((code_point >> (6 * 2)) & 0x3f) | 0xe0;
        code_unit->code[1] = ((code_point >> 6) & 0x3f) | 0x80;
        code_unit->code[2] = (code_point & 0x3f) | 0x80;
        break;
    case 4:
        code_unit->code[0] = ((code_point >> (6 * 3)) & 7) | 0xf0;
        code_unit->code[1] = ((code_point >> (6 * 2)) & 0x3f) | 0x80;
        code_unit->code[2] = ((code_point >> 6) & 0x3f) | 0x80;
        code_unit->code[3] = (code_point & 0x3f) | 0x80;
        break;
    }
}

int encode(uint32_t code_point, CodeUnit* code_unit)
{
    if (get_count_byte(code_point, code_unit) == -1) {
        return -1;
    }
    choose_your_pattern(code_point, code_unit);
    /*
    printf("\nlength : %ld\n", code_unit->length);
    for (size_t i = 0; i < code_unit->length; i++) {
            printf("Byte %ld : %d\n", i + 1, code_unit->code[i]);
    }	*/

    return 0;
}

uint32_t decode(const CodeUnit* code_unit)
{
    switch (code_unit->length) {
    case 1:
        return code_unit->code[0];
    case 2:
        return ((code_unit->code[0] & 0x1f) << 6)
                | ((code_unit->code[1] & 0x3f));
    case 3:
        return ((code_unit->code[0] & 0xf) << 12)
                | ((code_unit->code[1] & 0x3f) << 6)
                | (code_unit->code[2] & 0x3f);
    case 4:
        return ((code_unit->code[0] & 0xf) << 18)
                | ((code_unit->code[1] & 0x3f) << 12)
                | ((code_unit->code[2] & 0x3f) << 6)
                | (code_unit->code[3] & 0x3f);
    }
    return -1;
}

int read_next_code_unit(FILE* bin, CodeUnit* code_unit)
{
    int shift = fread(code_unit->code, sizeof(uint8_t), 4, bin);
    code_unit->length = 0;
    for (size_t i = 7; i > 3; i--) {
        if (((code_unit->code[0] >> i) & 1) == 1) {
            code_unit->length++;
        } else {
            break;
        }
    }
    switch (code_unit->length) {
    case 0:
        fseek(bin, -shift + 1, SEEK_CUR);
        code_unit->length++;
        return 0;
    case 2:
        fseek(bin, -shift + 2, SEEK_CUR);
        if ((((code_unit->code[1] >> 7) & 1) == 1)
            && (((code_unit->code[1] >> 6) & 1) == 0)) {
            return 0;
        }
    case 3:
        fseek(bin, -shift + 3, SEEK_CUR);
        if ((((code_unit->code[1] >> 7) & 1) == 1)
            && (((code_unit->code[1] >> 6) & 1) == 0)
            && (((code_unit->code[2] >> 7) & 1) == 1)
            && (((code_unit->code[2] >> 6) & 1) == 0)) {
            return 0;
        }
    case 4:
    	fseek(bin, -shift + 3, SEEK_CUR);
        if ((((code_unit->code[1] >> 7) & 1) == 1)
            && (((code_unit->code[1] >> 6) & 1) == 0)
            && (((code_unit->code[2] >> 7) & 1) == 1)
            && (((code_unit->code[2] >> 6) & 1) == 0)
            && (((code_unit->code[3] >> 7) & 1) == 1)
            && (((code_unit->code[3] >> 6) & 1) == 0)) {
            return 0;
        }
    }

    return -1;
}

void write_code_unit(FILE* out, uint32_t value)
{
    fprintf(out, "%" PRIx32 "\n", value);
}
