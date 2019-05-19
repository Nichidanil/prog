#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

enum { MaxCodeLength = 4 };

typedef struct {
    uint8_t code[MaxCodeLength];
    size_t length;
} CodeUnit;

void choose_your_pattern(uint32_t code_point, CodeUnit* code_unit);
int get_count_byte(uint32_t code_point, CodeUnit* code_unit);
int encode(uint32_t code_point, CodeUnit* code_unit);
uint32_t decode(const CodeUnit* code_unit);
int read_next_code_unit(FILE* in, CodeUnit* code_unit);
void write_code_unit(FILE* out, uint32_t value);
