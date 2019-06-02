#include "string.h"
#include <stdio.h>
#include <stdlib.h>

void input(char* paths, char* delim) 
{
	char temp[3];
    fputs("delim: ", stdout);
    fgets(temp, 3, stdin);
    *delim = *temp;
    fputs("paths: ", stdout);
    fgets(paths, MAX_PATH * 10 + 5, stdin);
}

int check(char* paths, char delim)
{
	/*Проверка корректности данных*/
	if (delim != ' ' && delim != '+' && delim != ':') {
		return 1;
	}

	char* path = (char *)malloc(sizeof(char) * MAX_PATH * 2);
	int index;
	while (slen(paths) > 0) {
		path = stok(paths, delim);
		index = slen(path) + 1;

		if (index - 1 > MAX_PATH) {
			free(path);
			return 3;
		}

		paths = scpy(&paths[index]);
	}
	

	int check_syn;
	check_syn = check_unix(paths);
	if (check_syn != 0) {
		free(path);
		return check_syn;
	}

	free(path);
	return 0;
}

int check_unix(char* paths)
{
	/*Проверка на запрещенные символы*/
	char unix_forb_char[] = "\\\0";

	int check_syn = sspn(paths, unix_forb_char);
	if (check_syn == -1) {
		return 2;
	}

	return 0;
}

char* process(char* paths, char delim)
{
	char slash = '/';
	char* result = (char *)malloc(sizeof(char) * MAX_PATH * 10 + 5);
	if (result == NULL) {
		return NULL;
	}
	char* path = (char *)malloc(sizeof(char) * MAX_PATH * 2);
	char array[MAX_PATH][MAX_PATH];
	char* token = (char *)malloc(sizeof(char) * MAX_PATH);
	if (token == NULL) {
		free(result);
		return NULL;
	}
	int index;
	int token_index;
	int i = 0;
	int j = 0;
	int k;
	int l = 0;
	while (1) {
		path = stok(paths, delim);
		index = slen(path) + 1;
		i = j;
		while (slen(path) > 0) {
			token = stok(path, slash);
			token_index = slen(token) + 1;
			for (k = 0; token[k] != '\0'; k++) {
				array[j][k] = token[k];
			}
			array[j][k] = '\0';
			if (scmp(array[j], "..\0") == 0 && j != 0) {
				j -= 2;
			} else if (scmp(array[j], ".\0") == 0) {
				j--;
			}
			j++;
			path = scpy(&path[token_index]);
		}

		k = 0;
		for (; i < j; i++) {
			for (int h = 0; array[i][h] != '\0'; h++, l++) {
				result[l] = array[i][h];
			}
			result[l] = '/';
			l++;
		}

		paths = scpy(&paths[index]);
		if (slen(paths) > 0) {
			result[l] = delim;
			l++;
		} else {
			break;
		}
	}

	free(path);
	free(token);
	return result;
}

int output(char* paths, char delim)
{
	/*Вывод результата*/
	char* result = process(paths, delim);
	if (result == NULL) {
		return 4;
	}
	printf("%s\n", result);
	free(result);
	return 0;
}

int slen(char* str)
{
	/*Длина строки*/
	int i;
	for (i = 0; str[i] != '\0'; i++) {}
	return i;
}

char* stok(char* str, char delim)
{
	/*Разбиение строки на элементы-токены*/
	char* token = (char *)malloc(sizeof(char) * MAX_PATH * 2);
	if (token == NULL || str == NULL) {
		return NULL;
	}
	for (int i = 0;; i++) {
		if (str[i] == delim || str[i] == '\0') {
			token[i] = '\0';
			return token;
		}
		token[i] = str[i];
	}
}

int sspn(char* str, char* forb_char)
{
	/*Проверка допустимости используемых символов*/
	for (int i = 0; str[i] != '\0'; i++) {
		for (int j = 0; j < slen(forb_char) + 1; j++) {
			if (str[i] == forb_char[j]) {
				return -1;
			}
		}
	}
	return 0;
}

int scmp(char *str1, char *str2) {
	int size1 = slen(str1);
	int size2 = slen(str2);
	if (size1 > size2) {
		return 1;
	} else if (size1 < size2) {
		return -1;
	}

    return 0;
}

char* scpy(char* str)
{
	/*Копирование str1 в str2*/
	if (str == NULL) {
		return NULL;
	}
	char* strCopy = (char *)malloc(sizeof(char) * MAX_PATH * 2);

	if (strCopy == NULL) {
		return NULL;
	}

	int i;
	for (i = 0; str[i] != '\0'; i++) {
		strCopy[i] = str[i];
	}
	strCopy[i] = '\0';

	return strCopy;
}

int delete_n(char* str)
{
	int index_n = slen(str) - 1;
	if (str[index_n] != '\n') {
		return 1;
	}
	str[index_n] = '\0';
	return 0;
}

void print_error(int error_numb)
{
	switch(error_numb) {
		case 1:
			printf("DELIM ERROR\n");
			break;
		case 2:
			printf("PATH ERROR\n");
			break;
		case 3:
			printf("MAX PATH ERROR\n");
			break;
		case 4:
			printf("MEMORY ALLOCATION ERROR\n");
			break;
	}
}
