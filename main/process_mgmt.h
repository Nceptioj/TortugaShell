/* process_mgmt.h
* Header file for process_mgmt.c
*/

#ifndef PROCESS_MGMT
#define PROCESS_MGMT

extern int create_process(char *, char *);
extern char *get_system_dir(void);
wchar_t *convert_to_wchar(char *);
char *convert_to_char(wchar_t *);

#endif
