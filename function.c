/* function.c */
#include "headfile.h"
#include "debug.h"
#include "config.h"
#include "token.h"

char str[STRINGSIZE]={0};	/* for read_v and fread_v */
extern char token_buffer[TOKENSIZE];

extern int  getNodeType(char *s);					/* database.c */
extern void putStringToNode(char *s, char *str);			/* database.c */
extern void putIntValToNode(char *s, int v);				/* database.c */
extern char *getStringFromNode(char *s);				/* database.c */
extern int  getIntValFromNode(char *s);					/* database.c */
extern char *getExpressionStringFromNode(char *s);			/* database.c */
extern int  next_token(void);						/* token.c */

extern void functionError(char *s);					/* error.c */
extern void fatal_functionError(char *s);				/* error.c */
/*================================*/
extern void addFunctionTable(void);
extern void invokeFunction(char *s);
void read_v(int t);
void write_v(int t);
void ln(void);

void show_vertbl(int t);
extern void kr_show_vertbl(char *s);					/* vertbl.c */
void show_vertblex(int t);
extern void kr_show_vertblex(char *s);					/* vertbl.c */
void factorization(int t);
extern void kr_factorization(char *s);					/* vertbl.c */
void show_dual(int t);
extern void kr_show_dual(char *s);					/* dual.c */
void equivalence_figure(int t);
//extern void kr_equivalence_figure(char *s);						
/* void fread_v(FILE *fp, char *s, int t, int v, char *d); */
/* void fwrite_v(FILE *fp, char *s, int t, int v, char *d); */
/*================================*/
/* void isEquPol(void); */
/* bool isOne(int id); */
/* bool isTwo(int id); */
/* bool isThree(int id); */
/* bool listTbl(int id); */
/*================================*/
void invokeFunction(char *s)
{  
   	DBGPRT("Function: invokeFunction\r\n");
   	DBGPRT1("invokeFunction: the argument: s= %s\r\n",s);
   	int token;
   	if(strcmp(s,"read")==0)
   	{
      		token=next_token();
      		if(token!=LPAREM)
      		{
         		functionError("read function lose (");
      		}/* end if */
      		token=next_token();  
         	read_v(token);
      		token=next_token();
      		if(token!=RPAREM)
      		{
         		/* error */
			functionError("read function lose )");
      		}/* end if */
   	}/* end if */
   	else if(strcmp(s,"write")==0)
   	{  
      		DBGPRT("invokeFunction: invoke write\r\n");
      		token=next_token();
      		DBGPRT1("invokeFunction: invoke write: token= %d\r\n",token);
      		if(token!=LPAREM)
      		{
         		/* end if */
			functionError("write function lose (");
      		}/* end if */
      		token=next_token();
      		DBGPRT1("invokeFunction: invoke write: token= %d\r\n",token);
         	write_v(token);
      		token=next_token();
      		DBGPRT1("invokeFunction: invoke write: token= %d\r\n",token);
      		if(token!=RPAREM)
      		{
         		/* error */
			functionError("read function lose )");
      		}/* end if */
     	 	DBGPRT("invokeFunction: exit invoke write\r\n");
   	}/* end else if */
	else if(strcmp(s,"writeln")==0)
	{
      		DBGPRT("invokeFunction: invoke writeln\r\n");		
		token=next_token();
	      	DBGPRT1("invokeFunction: invoke writeln: token= %d\r\n",token);	
      		if(token!=LPAREM)
      		{
         		/* end error */
			functionError("writeln function lose (");
      		}/* end if */
		token=next_token();
		write_v(token);
		ln();
		token=next_token();
      		if(token!=RPAREM)
      		{
         		/* error */
			functionError("writeln function lose )");
      		}/* end if */
     	 	DBGPRT("invokeFunction: exit invoke writeln\r\n");	
	}/* end else if */
   	else if(strcmp(s,"ln")==0)
   	{
      		ln(); 
   	}/* end else if */
	else if((strcmp(s,"show_vertbl")==0)||(strcmp(s,"svt")==0))
	{
		token=next_token();
		if(token!=LPAREM)
		{
			/* error */
			functionError("show_vertbl or svt function lose (");
		}/* end if */
		token=next_token();
		show_vertbl(token);
		token=next_token();
		if(token!=RPAREM)
		{
			/* error */
			functionError("show_vertbl or svt function lose )");
		}/* end if */
	}/* end else if */
	else if((strcmp(s,"show_vertblex")==0)||(strcmp(s,"svtex")==0))
	{
		token=next_token();
		if(token!=LPAREM)
		{
			/* error */
			functionError("show_vertblex or svtex function lose (");
		}/* end if */
		token=next_token();
		show_vertblex(token);
		token=next_token();
		if(token!=RPAREM)
		{
			/* error */
			functionError("show_vertblex or svtex function lose )");
		}/* end if */		
	}/* end else if */
	else if((strcmp(s,"factorization")==0)||(strcmp(s,"fct")==0))
	{
		token=next_token();
		if(token!=LPAREM)
		{
			/* error */
			functionError("factorization or fct function lose (");
		}/* end if */
		token=next_token();
		factorization(token);
		token=next_token();
		if(token!=RPAREM)
		{
			/* error */
			functionError("factorization or fct function lose )");
		}/* end if */
	}/* end else if */
	else if((strcmp(s,"show_dual")==0)||(strcmp(s,"sdl")==0))
	{
		token=next_token();
		if(token!=LPAREM)
		{
			/* error */
			functionError("show_dual or sdl function lose (");
		}/* end if */
		token=next_token();
		show_dual(token);
		token=next_token();
		if(token!=RPAREM)
		{
			/* error */
			functionError("show_dual or sdl function lose )");
		}/* end if */		
	}/* end else if */
	else if((strcmp(s,"equivalence_figure")==0)||(strcmp(s,"equfig")==0))
	{
		token=next_token();
		if(token!=LPAREM)
		{
			/* error */
			functionError("equivalence_figure or equfig function lose (");
		}/* end if */
		token=next_token();
		/* equivalence_figure(token); */
		token=next_token();
		if(token!=RPAREM)
		{
			/* error */
			functionError("equivalence_figure or equfig function lose )");
		}/* end if */
	}/* end else if */
   	else if(strcmp(s,"fread")==0)
   	{
   	}/* end else if */
   	else if(strcmp(s,"fwrite")==0)
   	{
   	}/* end else if */
   	else if(strcmp(s,"isone")==0)
   	{
   	}/* end else if */
   	else if(strcmp(s,"istwo")==0)
   	{
   	}/* end else if */
   	else if(strcmp(s,"isthree")==0)
   	{
   	}/* end else if */
   	else
   	{
      		/* error */
		printf("error: the function is undefine\r\n");
		return;
   	}/* end else */
   	DBGPRT("End invokeFunction\r\n");
   	return;
}/* end invokeFunction */

void write_v(int t)
{ 
  	DBGPRT("Function: write_v\r\n");
   	DBGPRT1("write_v: the argument: t= %d\r\n",t);
  	int  value;
 	char *str;
  	if((t==ATOMTYPE)||(t==COMPLEXTYPE))
  	{   
     		value=getIntValFromNode(token_buffer);
     		printf("%d",value);
  	}/* end if */
  	else if(t==STRINGTYPE)
  	{
    	 	str=getStringFromNode(token_buffer);
     		printf("%s",str);
  	}/* end else if */
  	else if(t==STRINGVAL)
  	{
     		str=token_buffer;
     		printf("%s",str);
  	}/* end else if */
  	else if(t==PREDICATETYPE)
  	{
  	}/* end else if */
  	else
  	{
     		/* error */
		fatal_functionError("invalid type on write function");
  	}/* end else */
  	DBGPRT("End write_v\r\n");
  	return;
}/* end write_v*/

void read_v(int t)
{ 
	DBGPRT("Function: read_v\r\n");
   	DBGPRT1("read_v: the argument: t= %d\r\n",t);
  	int v=-1;
  	char c;
  	if((t==ATOMTYPE)||(t==COMPLEXTYPE))
  	{  
     		do{
         		c=getchar();
         		if(c=='T'){v=TRUE;}
         		else if(c=='F'){v=FALSE;}
         		else{v=-1;}
         		if((v!=TRUE)&&(v!=FALSE))
         		{  
            			printf("invalid value,please input T or F \r\n");
            			c=getchar();
            			if(c=='T'){v=TRUE;}
            			else if(c=='F'){v=FALSE;}
            			else{v=-1;}
         		}/* end if */
     		}while((v!=TRUE)&&(v!=FALSE));
     		putIntValToNode(token_buffer,v);
  	}/* end if */
  	else if(t==STRINGTYPE)
  	{  
     		memset(str,0,STRINGSIZE);
     		scanf("%s\r\n",str);
     		putStringToNode(token_buffer, str);
  	}/* end else if */
  	else if(t==PREDICATETYPE)
  	{
  	}/* end else if */
  	else
  	{
     		/* error */
		fatal_functionError("invalid type on read function");
  	}/* end else */
  	DBGPRT("End read_v\r\n");
  	return;
}/* end read_v */

void show_vertbl(int t)
{	
	if(t!=COMPLEXTYPE)
	{
		return;
	}/* end if */
	kr_show_vertbl(getExpressionStringFromNode(token_buffer));
	return;
}/* end show_vertbl */

void show_vertblex(int t)
{
	if(t!=COMPLEXTYPE)
	{
		return;
	}/* end if */
	kr_show_vertblex(getExpressionStringFromNode(token_buffer));
	return;	
}/* end show_vertblex */

void factorization(int t)
{
	if(t!=COMPLEXTYPE)
	{
		return;
	}/* end if */
	kr_factorization(getExpressionStringFromNode(token_buffer));
	return;		
}/* end factorization */

void show_dual(int t)
{
	if(t!=COMPLEXTYPE)
	{
		return;
	}/* end if */
	kr_show_dual(getExpressionStringFromNode(token_buffer));
	return;		
}/* end show_dual */

void equivalence_figure(int t)
{
	if(t!=COMPLEXTYPE)
	{
		return;
	}/* end if */
	/* kr_equivalence_figure(getExpressionStringFromNode(token_buffer)); */
	return;
}/* end equivalence_figure */

void ln(void)
{
  DBGPRT("Function: ln\r\n");
  printf("\r\n");
  DBGPRT("End ln\r\n");
  return;
}/* end ln */
