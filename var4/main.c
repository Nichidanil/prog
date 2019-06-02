#include "string.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{	
	// EXAMPLE
	// /home/../alex/../video/../progs/.././test.c+~alex/docs+~/study/../Prog/./lab4.c+/usr/../bin/../gcc/../
	char delim;
	char* paths = (char *)malloc(sizeof(char) * MAX_PATH * 10 + 5);

	input(paths, &delim);

	delete_n(paths);

	int err_numb = check(paths, delim);
	if (err_numb != 0) {
		print_error(err_numb);
		return -1;
	}
	
	err_numb = output(paths, delim);
	if (err_numb != 0) {
		print_error(err_numb);
		return err_numb;
	}

	free(paths);

	return 0;
}