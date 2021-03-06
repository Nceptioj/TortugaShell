/*
* myStrings.c
*
*  Created on: 14/07/2014
*      Author: Fraser
*
*  Contains methods which help with string manipulation.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <wchar.h>
#include "myStrings.h"
#include "shell.h" //for debug_global

/* -----WINDOWS----
* Concatenates up to three strings.
* Parameter: First string, second string, third string (or null).
* Return: Resulting concatenation.
*/
wchar_t *concat_string(wchar_t *first, wchar_t *second, wchar_t *third){
	size_t first_len = wcslen(first) + 1;
	size_t second_len = wcslen(second) + 1;
	size_t third_len = 0;

	if (debug_global > 1){ wprintf(L"CONCAT_STRING: Input: %s %s\n", first, second); }
	if (third){
		if (debug_global > 1){ wprintf(L"CONCAT_STRING: Input: %s\n", third); }
		third_len = wcslen(third) + 1;
	}

	wchar_t *result = emalloc(sizeof(wchar_t) * (first_len + second_len + third_len));

	if (debug_global > 1){ wprintf(L"CONCAT_STRING: Adding first: %s\n", first); }
	if (!wcsncpy(result, first, first_len)){
		fwprintf(stderr, L"CONCAT_STRING: Error copying first string to result.\n");
		return 0;
	}
	if (debug_global > 1){ wprintf(L"CONCAT_STRING: Adding second: %s\n", second); }
	if (!wcsncat(result, second, second_len)) {
		fwprintf(stderr, L"CONCAT_STRING: Error copying second string to result.\n");
		return 0;
	}

	if (third){
		if (debug_global > 1) { wprintf(L"CONCAT_STRING: Adding third: %s\n", third); }
		wcsncat(result, third, third_len);
	}

	if (debug_global > 1) { wprintf(L"CONCAT_STRING: Returning: %s\n", result); }
	return result;
}

/* -----WINDOWS----
* Splits a string of space seperated words into an array of words
* Parameter: String to split, delimiter, memory address of integer to store index of last item.
* Return: Array of words
*/
wchar_t **split(wchar_t *str, wchar_t *delimiter, int *last_index) {
	int count = 0;
	int length;
	int we_are_in_quotations = 0;
	wchar_t **commands = 0;
	wchar_t *newline;
	wchar_t *token = wcstok(str, delimiter);
	wchar_t *long_string = L"";
	wchar_t *third_thing = NULL;
	wchar_t *space = L" ";

	if (debug_global){ wprintf(L"SPLIT: Input: %s\n", str); }

	while (token) {
		newline = wcschr(token, L'\n');

		if (newline) {
			*newline = 0;
		}

		length = wcslen(token);
		if (debug_global){ wprintf(L"SPLIT: Working on token: %s of length %d\n", token, length); }

		if (token[0] == L'"'){
			if (debug_global){ wprintf(L"SPLIT: Quotations started\n"); }
			long_string = L"";
			we_are_in_quotations = 1;
			token = token + 1;
			third_thing = space;
			length--;
		} 
		
		if (token[length - 1] == L'"'){
			if (debug_global){ wprintf(L"SPLIT: Quotations ended\n"); }
			we_are_in_quotations = 0;
			token[length - 1] = L'\0';
			third_thing = NULL;
		}
		
		if (we_are_in_quotations){
			third_thing = space;
		}

		long_string = concat_string(long_string, token, third_thing);
		
		if (!we_are_in_quotations){
			commands = erealloc(commands, sizeof(wchar_t*)* ++count);
			commands[count - 1] = _wcsdup(long_string);
			if (debug_global){ wprintf(L"SPLIT: Done with token: %s\n", commands[count - 1]); }
			long_string = L"";
		}

		token = wcstok(0, delimiter);
	}

	//Add a null entry to the end of the array
	commands = erealloc(commands, sizeof (wchar_t*)* (count + 1));
	commands[count] = 0;
	free(token);

	if (debug_global){ wprintf(L"SPLIT: Returning %i tokens\n", count); }
	*last_index = count;
	return commands;
}

/* -------WINDOWS------
* Converts a normal array of char into an array of wide char because Windows
* Parameter: String to convert
* Return: Wchar version of input
*/
wchar_t *convert_to_wchar(char *input){
	if (debug_global > 1) { printf("CONVERT_TO_WCHAR: Input - %s\n", input); }

	size_t len = strlen(input) + 1;
	wchar_t *command_w = emalloc(sizeof(wchar_t)* len);

	swprintf(command_w, len, L"%hs", input);
	if (debug_global > 1) wprintf(L"CONVERT_TO_WCHAR: Output - %s\n", command_w);
	return command_w;
}

/* -------WINDOWS------
* Converts an array of widechar into an array of char because Windows
* Parameter: String to convert
* Return: Wchar version of input
*/
char *convert_to_char(wchar_t *input){
	if (debug_global > 1){ wprintf(L"CONVERT_TO_CHAR: Input - %ws\n", input); }

	size_t len = wcslen(input) + 1;
	char *command_c = emalloc(sizeof(char)* len);

	wcstombs(command_c, input, len);
	if (debug_global > 1){ printf("CONVERT_TO_CHAR: Output - %s\n", command_c); }
	return command_c;
}
