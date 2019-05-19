#pragma once
#include <inttypes.h>
#include <stddef.h>

int encode_file(const char* in_file_name, const char* out_file_name);
int decode_file(const char* in_file_name, const char* out_file_name);
