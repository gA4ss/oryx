/* error.c */
/* #include "error.h" */ 
#include "config.h"

/* FILE *err_fp; */
/* char errname[MAX_PATH]={0}; */

extern void lexicalError(char *s);
extern void fatal_lexicalError(char *s);
extern void syntaxError(char *s);
extern void fatal_syntaxError(char *s);
extern void fatal_cnfof(char *s);
extern void functionError(char *s);
extern void fatal_functionError(char *s);

void printfErrorCount(void);

int err_count=0;
unsigned long line_count;

void lexicalError(char* s)
{	
	printf("<%d> lexical error: %s.\r\n", line_count, s);
	err_count++;
	return;
}/* end lexicalError */

void syntaxError(char *s)
{	
	printf("<%d> syntax error: %s.\r\n", line_count, s);
	err_count++;
	return;
}/* end syntaxError */

void fatal_lexicalError(char *s)
{	
	printf("<%d> lexical fatal error: %s.\r\n", line_count, s);
	err_count++;
	printfErrorCount();
	exit(-1);
}/* end fatal_lexicalError */

void fatal_syntaxError(char *s)
{	
	printf("<%d> syntax fatal error: %s.\r\n", line_count, s);
	err_count++;
	printfErrorCount();
	exit(-1);	
}/* end fatal_syntaxError */

void fatal_cnfof(char *s)
{
	err_count++;
	printf("oryx : fatal error: can not open source file: \"%s\": No such file", s);
	exit(-1);
}/* end fatal_cnfof */

void functionError(char *s)
{
	printf("<%d> function error: %s.\r\n", line_count, s);
	err_count++;
	return;	
}/* end functionError */
void fatal_functionError(char *s)
{
	printf("<%d> function fatal error: %s.\r\n", line_count, s);
	err_count++;
	printfErrorCount();
	exit(-1);	
}/* end fatal_functionError */

void printfErrorCount(void)
{
	printf("the total of error : %d\r\n", err_count);
	return;
}/* end printfErrorCount */

