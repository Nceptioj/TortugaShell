/*
* rmdir.c
*
* Created: 07/08/2014
* Author: James Buchanan
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

/* Remove a directory */
int wmain(int argc, wchar_t *argv[]){
	int i = 0;
	//Help message, printed by default if no arguments
	while (argv[i]){
		if (argc == 1 || ((wcscmp(argv[i], L"-help") == 0) || (wcscmp(argv[i], L"-h") == 0))) {
			wprintf(L"rmdir\tPermanently deletes a directory.\n\tUsage: rmdir [dir]\n");
			return EXIT_SUCCESS;
		}
		i++;
	}

	if (argv[1] == 0){
		return EXIT_FAILURE;
	}

	if (RemoveDirectory(argv[1]) == 0) {
		wprintf(L"\nCouldn't remove %s directory.\n", argv[1]);
	}
	else {
		wprintf(L"\n%s directory removed successfully.\n", argv[1]);
	}

	return EXIT_SUCCESS;
}