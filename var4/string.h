#pragma once

#define MAX_PATH 260

int output(char* path, char delim);
void print_error(int error_numb);
int delete_n(char* str);
int check_unix(char* path);
void input(char* paths, char* delim);
int check(char* path, char delim);
char* stok(char* str, char delim);
char* scpy(char* str);
int slen(char* str);
int sspn(char* str, char* forb_char);
int scmp(char *str1, char *str2);
char* process(char* path, char delim);
