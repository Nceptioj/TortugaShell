/*
* cat.c
* Created 14/05/14
* Author: LeYing Tran
* Copies the contents of the given file
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -----CROSS-PLATFORM----
* Copies the file from the first argument.
*/
int main(int argc, char *argv[]){
	FILE *fp;
	int i = 0;

	//Help message, printed by default if no arguments
	while (argv[i]){
		if (argc == 1 || ((strcmp(argv[i], "-help") == 0) || (strcmp(argv[i], "-h") == 0))) {
			printf("cat\tPrints the content of the requested file to the commandline.\n\tUsage: cat [file]\n");
			return EXIT_SUCCESS;
		}
		i++;
	}

	if (argv[1]){
		if ((fp = fopen(argv[1], "r")) != NULL){
			char line[256];
			while (fgets(line, 256, fp)){
				printf("%s\n", line);
			}
			fclose(fp);
			return EXIT_SUCCESS; //this is 0
		}
		else
		{
			printf("CAT: Error opening file.\n");
		}
	}

	return EXIT_FAILURE; //this is 1
}
