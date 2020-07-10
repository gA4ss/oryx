/* run.c */
#include "headfile.h"
#include "config.h"
#include "debug.h"
#include "cmdline.h"

/* char input; */
/* function */
extern void run(cmdline *cl);
extern void analyzeLogicSyntax(void);	/* syntax.c */
extern void openLgxFile(char *s);					/* token.c */
extern void closeLgxFile(char *s);					/* token.c */
extern void initHashTree(void);						/* database.c */
extern void freeHashTree(void);						/* database.c */

int token_ram_len=0;
int compute_ram_len=0;
int analyze_string_len=0;
int symbol_atom_number=0;
int equfigure_list_len=0;
char **object_symbol;
char **equ_figure_symbol;

void run(cmdline *cl)
{ 
  	/* start */
	int i;
  	strcat(cl->filename,ORYXFILE);
	token_ram_len=cl->token_ram_len;
	compute_ram_len=cl->compute_ram_len;
	analyze_string_len=cl->analyze_string_len;
	symbol_atom_number=cl->symbol_atom_number;
	equfigure_list_len=cl->equfigure_list_len;

	object_symbol=(char **)malloc(sizeof(char *) * symbol_atom_number);
	for(i=0;i<symbol_atom_number;i++)
	{
		object_symbol[i]=(char *)malloc(analyze_string_len * sizeof(char));
		memset(object_symbol[i], 0, analyze_string_len);
	}/* end for */
	
	equ_figure_symbol=(char **)malloc(sizeof(char *) * equfigure_list_len);
	for(i=0;i<equfigure_list_len;i++)
	{
		equ_figure_symbol[i]=(char *)malloc(analyze_string_len * sizeof(char));
		memset(equ_figure_symbol[i], 0, analyze_string_len);
	}/* end for */

  	openLgxFile(cl->filename);
	/*
  	printf("\r\n");
  	printf("OryX Version %s\r\n",VERSION); 
  	printf("View http://www.prison4th.com/ to download new version\r\n");
  	printf("If you found some bugs or have some good ideas\r\n");
  	printf("Please mail to logic.yan@gmail.com\r\n\r\n");
  	printf("Open file %s... Completed\r\n",cl->filename);
	*/
  	initHashTree();
  	/* printf("Initilize symbol table... Completed\r\n"); */
	/*
		addFunctionTable(); 
  		printf("Add function name to symbol table... Completed\r\n"); 
	*/

  	/*
		addDefaultNodeName();
		printf("Add default node name to symbol table... Completed\r\n");
  	*/
	/*
  	printf("Please press any key to start\r\n");
  	scanf("%c",&input);
  	printf("START\r\n");
	*/
  	analyzeLogicSyntax();
  	/* printf("END\r\n"); */
  	freeHashTree();
	/*
  	printf("Free symbol table... Completed\r\n");
	*/
  	closeLgxFile(cl->filename);
	/*
  	printf("Close file %s... Completed\r\n",cl->filename);
  	printf("Analyze logix script... Completed\r\n\r\n");
  	printf("Please press any key to end\r\n");
  	scanf("%c",&input);
	*/
	for(i=0;i<symbol_atom_number;i++)
	{
		if(object_symbol[i]!=NULL)free(object_symbol[i]);
	}/* end for */
	if(object_symbol!=NULL)free(object_symbol);

	for(i=0;i<equfigure_list_len;i++)
	{
		if(equ_figure_symbol[i]!=NULL)free(equ_figure_symbol[i]);
	}/* end for */
	if(equ_figure_symbol!=NULL)free(equ_figure_symbol);

  	return;
}/* end run */




