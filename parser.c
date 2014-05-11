/*
 * parser.c
 *
 *  Created on: 14/04/2014
 *      Author: Fraser
 */
#define _CRT_SECURE_NO_WARNINGS
#define DEBUG 1

#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "parser.h"
#include "process_mgmt.h"

/* -----CROSS PLATFORM----
* Check to see what the command type is: Whether it's just a single command or a physical path. 
* This is Windows so it does this by checking if the second character is a semicolon. Will have to be different for Linux.
* Parameter: Command to check.
* Return: An integer indicating whether it's a command (0) or a path (1)
*/
static int command_type(char *command){
	if (DEBUG){ printf("COMMAND_TYPE: Input: %s\n", command); }
	if (command[1] == ':'){
		if (DEBUG){ printf("COMMAND_TYPE: It's a path.\n", command); }
		return 1;
	}
	if (DEBUG){ printf("COMMAND_TYPE: It's a command.\n", command); }
	return 0;
}

/* -----CROSS PLATFORM----
* Concatenates up to three strings.
* Parameter: First string, second string, third string.
* Return: Resulting concatenation.
*/
char *concat_string(char *first, char *second, char *third){
	size_t first_len = strlen(first) + 1;
	size_t second_len = strlen(second) + 1;
	size_t third_len = 0;

	if (DEBUG){ printf("CONCAT_STRING: Input: %s %s\n", first, second); }
	if (third){
		if (DEBUG){ printf("CONCAT_STRING: Input: %s\n", third); }
		third_len = strlen(third) + 1;
	}

	char *result = (char*)malloc(first_len + second_len + third_len);

	if (result == NULL){
		fprintf(stderr, "CONCAT_STRING: Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	if (DEBUG){ printf("CONCAT_STRING: Adding first: %s\n", first); }
	strncpy(result, first, first_len);
	if (DEBUG){ printf("CONCAT_STRING: Adding second: %s\n", second); }
	strncat(result, second, second_len);

	if (third){
		if (DEBUG) { printf("CONCAT_STRING: Adding third: %s\n", third);  }
		strncat(result, third, third_len);
	}

	if (DEBUG) { printf("CONCAT_STRING: Returning: %s\n", result); }
	return result;
}

/* -----CROSS PLATFORM----
* Splits a string of space seperated words into an array of words
* Parameter: String to split
* Return: Array of words
*/
char **split(char *str) {
	char *token;
	char **commands = 0;
	char *newline;
	int count = 0;

	token = strtok(str, " ");

	if (DEBUG){ printf("SPLIT: Input: %s\n", str); }

	while (token) {
		if (DEBUG){ printf("SPLIT: Working on token: %s\n", token); }

		// Remove newline character
		newline = strchr(token, '\n');
		if (newline) {
			*newline = 0;
		}

		commands = realloc(commands, sizeof(char*)* ++count);
		if (commands == NULL){
			printf("SPLIT: Error during realloc!");
			exit(EXIT_FAILURE);
		}
		commands[count - 1] = token;
		token = strtok(0, " ");
		if (DEBUG){ printf("SPLIT: Done with token: %s\n", commands[count - 1]); }
	}

	//Add a null entry to the end of the array
	commands = realloc(commands, sizeof (char*)* (count + 1));

	if (commands == NULL){
		fprintf(stderr, "SPLIT: Error during realloc!");
		exit(EXIT_FAILURE);
	}

	commands[count] = 0;
	if (DEBUG){ printf("SPLIT: Returning\n"); }
	return commands;
}


/* -----CROSS PLATFORM----
* Parses a single command
* Parameters: Command to parse, array of params, type of command)
*/
void parse_command(char *command, char *params, int type) {
	int error = 0;
	char *command_dir = command;
	char *command_exe = command;
	char *dir = "./commands/";
	char *exe = ".exe";
	if (DEBUG){ printf("PARSE_COMMAND: Input: %s %s %i\n", command, params, type); }
	//do some stuff here so there's an array of dirs to search including %path%

	// Processing for single command
	if (type == 0) {
		// Add .exe on the end if not there already
		if (!strstr(command, exe)){
			command_exe = concat_string(command, exe, NULL);
		}
		// Add ./commands/ to the beginning so we can check there first
		command_dir = concat_string(dir, command_exe, NULL);
	}


	// Spawn the command
	if (DEBUG){ printf("PARSE_COMMAND: Trying to create %s as a process with params %s\n", command_dir, params); }
	error = create_process_win(command_dir, params);

	if (error == 0) {
		return;
	}

	// If the command isn't found
	if (error == 2 || error == 3){
		// Try again in the CWD
		if (type == 0){
			if (create_process_win(command, params) == 0){
				return;
			}
		}
		printf("'%s' does not exist.\n", command);
	}
}

/* -----CROSS PLATFORM----
* Processes a line of commands
* Parameter: Line to process
*/
void parse(char *cmdline) {
	char **commands;
	char *params = NULL;
	int type;
	int i = 2;

	if (DEBUG){ printf("PARSE: Input: %s\n", cmdline); }
	commands = split(cmdline);
	if (DEBUG){ printf("PARSE: First command: %s\n", commands[0]); }
	type = command_type(commands[0]);

	// If there's more than just one thing
	if (commands[1]){
		if (DEBUG){ printf("PARSE: Adding parameter: %s\n", commands[1]); }
		params = (char*)malloc(strlen(commands[1] + 1));
		if (params == NULL){
			fprintf(stderr, "PARSE: Error during malloc!");
		}
		strcpy(params, commands[1]);
		
		while (commands[i]){
			if (DEBUG){ printf("PARSE: Adding parameter: %s\n", commands[i]); }
			params = concat_string(params, " ", commands[i]);
			i++;
		}
	}
	parse_command(commands[0], params, type);
}
