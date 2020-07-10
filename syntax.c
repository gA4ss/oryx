/* syntax.h */
#include "debug.h"
#include "config.h"
#include "alloc.h"
#include "token.h"
#include "maths.h"
/* Data */
extern int token_ram_len;
extern int compute_ram_len;
extern int analyze_string_len;
extern int symbol_atom_number;

int token;
char program_name[128]={0};
char id_name[TOKENSIZE]={0};
char *expression_buffer;		
char *list_buffer;			

extern char token_buffer[TOKENSIZE];				/* token.c */

/* Function */
extern int  next_token(void);                   		/* token.c */	
extern void invokeFunction(char *s);            		/* function.c */

extern void addNodeToTree(char *s, int t);			/* database.c */
extern int  getNodeType(char *s);				/* database.c */
extern void putStringToNode(char *s, char *str);		/* database.c */
extern void putIntValToNode(char *s, int v);			/* database.c */
extern char *getStringFromNode(char *s);			/* database.c */
extern int  getIntValFromNode(char *s);				/* database.c */
extern void putExpressionToNode(char *s, char *e);		/* database.c */

extern void syntaxError(char *s);				/* error.c */
extern void fatal_syntaxError(char *s);				/* error.c */

void codebody(void);
void stringer(char *s);
void procedure(char *s);
void maths_logic_compute(char *s);
void logic_pred(char *s);
int maths_logic_clause(void);
int maths_logic_expression(void);

extern void analyzeLogicSyntax(void);		/* brain.h */

/* function */
void analyzeLogicSyntax(void)
{
   	/* set something */
   	DBGPRT("Function: analyzeLogicSyntax\r\n");

   	codebody();
   	token=next_token();
   	if(token!=SCANEOF)
   	{
      		/* error */
      		DBGPRT("ONE END IT IS NOT SCAEOF TOKEN\r\n");
		syntaxError("SCANEOF lose");
   	}/* end if */
   	DBGPRT("End analyzeLogicSyntax\r\n");
   	return;
}/* end analyzeLogicSyntax */

void stringer(char *s)    
{
   	DBGPRT("Function: stringer\r\n");
   	DBGPRT1("stringer: the argument: s= %s\r\n",s);
	memset(id_name,0,TOKENSIZE);
	strcpy(id_name,s);
   	token=next_token();
   	if(token!=ASSIGNOP)
   	{
      		/* error */
		syntaxError("handle string variable lose =");
   	}
   	token=next_token();
   	if(token!=STRINGVAL)
   	{
      		/* error */
		fatal_syntaxError("not string type");
   	}
   	/* putStringToNode */
	putStringToNode(id_name,token_buffer);
   	token=next_token();
   	if(token!=SEMICOLON)
   	{
      		/* error */
		syntaxError("lose ;");
   	}
   	DBGPRT("End stringer\r\n");
   	return;
}/* end stringer */

void maths_logic_compute(char *s) 
{
   	DBGPRT("Function: maths_logic_compute\r\n");
   	DBGPRT1("maths_logic_compute: the argument: s= %s\r\n",s);
	memset(id_name,0,TOKENSIZE);
	strcpy(id_name,s);
   	token=next_token();
   	if(token!=ASSIGNOP)
   	{
		/* error */
		syntaxError("compute complex lose =");
   	}/* end if */
   	int value;
   	value=maths_logic_expression();
	/* 
		add table list 
	*/
   	DBGPRT1("maths_logic_compute: maths_logic_expression= %d\r\n",value);
   	putIntValToNode(id_name,value);
	putExpressionToNode(id_name,expression_buffer);
   	DBGPRT("End maths_logic_compute\r\n");
   	return;
}/* end maths_logic_compute */

void procedure(char *s)	
{
   	DBGPRT("Function: procedure\r\n");
   	DBGPRT1("procedure: the argument: s= %s\r\n",s);
	memset(id_name,0,TOKENSIZE);
	strcpy(id_name,s);
   	invokeFunction(id_name);
   	token=next_token();
   	if(token!=SEMICOLON)
   	{
		/* error */
		syntaxError("handle procedure lose ;");
   	}/* end if */
   	DBGPRT("End procedure\r\n");
	return;
}/* end procedure */

void logic_pred(char *s)
{
	return;
}/* end logic_pred */

void codebody(void)
{
   	DBGPRT("Function: codebody\r\n");
	list_buffer=A_S(analyze_string_len);
	expression_buffer=A_S(analyze_string_len);
   	token=next_token();
   	if(token==LOGICLABEL)
   	{
      		strcpy(program_name,token_buffer);
      		DBGPRT1("program_name= %s\r\n",program_name);
      		token=next_token();
      		if(token!=BLPAREM)
      		{
          		/* error */
			syntaxError("handle complex lose (");
      		}/* end if */
      		token=next_token();
      		while(token!=BRPAREM)
      		{	
            		if((token==ATOMTYPE)||(token==COMPLEXTYPE))
            		{
				addNodeToTree(token_buffer,token);
                		maths_logic_compute(token_buffer);
            		}/* end if */
            		else if(token==PREDICATETYPE)
            		{
				addNodeToTree(token_buffer,token);
                		logic_pred(token_buffer);
            		}/* end else if */
            		else if(token==STRINGTYPE)
            		{
				addNodeToTree(token_buffer,token);
                		stringer(token_buffer);
            		}/* end else if */
            		else if(token==PROCEDURETYPE)
            		{
                		procedure(token_buffer);
            		}/* end else if */
            		else
            		{
               			/* error */
				syntaxError("invalid code type");
            		}
         		token=next_token();
		}/* end while */
   	}/* end else */
   	else
   	{	
      		/* error */
		fatal_syntaxError("miss main enter");
   	}/* end else */
	free(list_buffer);
	free(expression_buffer);
   	DBGPRT("End codebody\r\n");
   	return;
}/* end codebody */

int maths_logic_clause(void)
{  
   	DBGPRT("Function: maths_logic_clause\r\n");
   	int value;
   	int *ram;
   	int i=0;
   	ram=(int *)(malloc(sizeof(int)*compute_ram_len));
   	memset(ram,SKIP,(sizeof(int)*compute_ram_len));   
   	while(1)
   	{  
      		token=next_token();
      		if(token==RPAREM)
      		{
         		break;
      		}/* end if */
      		else if(token==LPAREM)
      		{
         		ram[i]=maths_logic_clause();
         		i++;
      		}/* end else if */
      		else if((token==ATOMTYPE)||(token==COMPLEXTYPE))
      		{
         		value=getIntValFromNode(token_buffer);
         		ram[i]=value;
         		i++;
      		}/* end else if */
      		else if((token>=NOT)&&(token<=IMP))
      		{
         		ram[i]=token;
         		i++;
      		}/* end else if */
      		else
      		{	
        		/* error */
			fatal_syntaxError("compute complex invalid operate");
      		}/* end else */
   	}/* end while */
   	ram[i]=END;
   	COMPUTE_COMPLEX(ram,value);
   	free(ram);
   	DBGPRT("End maths_logic_clause\r\n");
   	return value;
}/* end maths_logic_clause */

int  maths_logic_expression(void)  
{					
    	DBGPRT("Function: maths_logic_expression\r\n");
    	int value;
    	int *ram;
    	int i=0;
    	ram=(int *)(malloc(sizeof(int)*compute_ram_len));
    	memset(ram,END,(sizeof(int)*compute_ram_len));
    	token=next_token();
    	if(token==TRUEVAL)
    	{
       		token=next_token();
       		if(token==SEMICOLON)
       		{
          		return TRUE;
       		}/* end if */
       		else{/* error */}
    	}/* end if */
    	else if(token==FALSEVAL)
    	{
       		token=next_token();
       		if(token==SEMICOLON)
       		{
          		return FALSE;
       		}/* end if */
       		else{/* error */}
    	}/* end else if */
    	while(1)
    	{   
       		if(token==SEMICOLON)
       		{  
          		ram[i]=END;
          		break;
       		}/* end if */ 
       		if(token==LPAREM)
       		{   
           		ram[i]=maths_logic_clause();
           		i++;
       		}/* end if */
       		else if((token==ATOMTYPE)||(token==COMPLEXTYPE))
       		{   
           		value=getIntValFromNode(token_buffer);
           		ram[i]=value;
           		i++;
       		}/* end else if */
       		else if((token>=NOT)&&(token<=IMP))
       		{   
           		ram[i]=token;
           		i++;
       		}/* end if */
       		else
       		{
           		/* error */
			fatal_syntaxError("compute complex invalid operate");
       		}/* end else */
       		token=next_token();
    	}/* end while */
    	COMPUTE_COMPLEX(ram,value);
    	free(ram);
    	DBGPRT1("maths_logic_expression: value= %d\r\n",value);
    	DBGPRT("End maths_logic_expression\r\n");
    	return value;
}/* end maths_logic_expression */
