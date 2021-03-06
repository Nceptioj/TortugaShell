/*
* rm.c
* Created 16/05/14
* Author J Faulkner
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -----CROSS PLATFORM----
* Deletes a file.
*/
int main(int argc, char *argv[]){
	int i = 0;
	FILE* createdfile = NULL;
	int status;


	//Help message, printed by default if no arguments
	while (argv[i]){
		if (argc == 1 || ((strcmp(argv[i], "-help") == 0) || (strcmp(argv[i], "-h") == 0))) {
			printf("rm\tPermanently deletes a file.\n\tUsage: rm [file]\n");
			return EXIT_SUCCESS;
		}
		i++;
	}

	if (argv[1]){
		status = remove(argv[1]);

		if (status == 0){
			printf("RM: Success! File %s has been deleted\n", argv[1]);
		}
		else
		{
			printf("RM: File delete failed. :(\n");
		}
	}
}
