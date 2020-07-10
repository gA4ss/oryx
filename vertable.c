/* vertable.h */
#include "headfile.h"
#include "debug.h"
#include "config.h"
#include "alloc.h"
#include "vertbl_maths_token.h"
#include "vertbl_maths.h"

#define SQUARE_TWO(M,V)		{\
					int i=0;\
					V=1;\
					if(M!=0)\
					{\
						while(i<M)\
						{\
							V=V*2;\
							i++;\
						}\
					}\
				}

#define SETACTIVESTR(S)		scaner_index=0;\
				scaner_str=S;

#define SGETC(S,C)		C=S[scaner_index];\
				scaner_index++;

#define UNSGETC			scaner_index--	

#define DEFAULT_S_POINT		scaner_index=0		

extern int token_ram_len;			/* run.c */
extern int analyze_string_len;			/* run.c */
extern int symbol_atom_number;			/* run.c */
extern char **object_symbol;			/* run.c */

char vttoken_buffer[TOKENSIZE]={0};
char vtin_char;
int  scaner_index=0;
char *scaner_str;
int  token;
char **basic_atom=NULL;	
int  **val_list=NULL;
int  remain_n=0;
int  n_val_list=0;
int  single_atom_number=0;
int  id;

extern int analyzeLogicString(char *s);		/* brain.h */
extern void initMemoryTree(void);		/* memory.h */
extern void freeMemoryTree(void);		/* memory.h */
extern int  getTypeFromId(int id);		/* memory.h */
extern int  getIdFromSrc(char *s);		/* memory.h */
extern int  *getTokenRamFromId(int id);		/* memory.h */
extern int  getTypeFromSrc(char *s);		/* memory.h */

extern void kr_show_vertbl(char *s);
extern void kr_show_vertblex(char *s);
extern void kr_factorization(char *s);

int  vtnext_token(void);
void atomListMaker(void);
void verTableMaker(int l);
int  maths_logic_expression_vt(int c);
int  maths_logic_clause_vt(int c);


/* function */
void atomListMaker(void)
{	
	/* make atom */
	int i;
	int j;
	int n;

	int min_n=0;
        int max_n=0;

	basic_atom=(char **)malloc(n_val_list*sizeof(char*));
	for(i=0;i<n_val_list;i++)
	{
		basic_atom[i]=A_S(single_atom_number+1);
		memset(basic_atom[i], 0, sizeof(char)*(single_atom_number+1));
	}/* end for */

	char *tmp=A_S(single_atom_number+1);
	int basic_len=0;
	for(i=0;i<n_val_list;i++)
	{	
		memset(tmp, 0, single_atom_number+1);
		_itoa(i, basic_atom[i], 2);
		basic_len=strlen(basic_atom[i]);
		if(basic_len != single_atom_number)
		{	
			n=single_atom_number - basic_len;
			j=0;
			while(j<n)
			{	
				tmp[j]='0';
				j++;
			}/* end while */
			n=0;
			while(j<single_atom_number)			
			{
				tmp[j]=basic_atom[i][n];
				j++;
				n++;
			}/* end while */
			strncpy(basic_atom[i], tmp, single_atom_number);
		}/* end if */	
	}/* end for */

	for(i=0; i<n_val_list; i++)
	{
		for(j=0; j<single_atom_number; j++)
		{
			if(basic_atom[i][j]=='0')
			{
				val_list[i][j]=0;
			}/* end if */
			else
			{
				val_list[i][j]=1;
			}/* end else */
		}/* end for */
	}/* end for */
	free(tmp);
	for(i=0;i<n_val_list;i++)
	{
		if(basic_atom[i]!=NULL)free(basic_atom[i]);
	}/* end for */
	if(basic_atom!=NULL)free(basic_atom);
	return;
}/* end atomListMaker */

void verTableMaker(int l)
{
	int type;
	int i;
	int j;
	/* get single atom number and insert */
	i=0;
	do
	{
		type=getTypeFromSrc(object_symbol[i]);
		i++;
	}while((i<l)&&(type==ATOM));
	single_atom_number=i-1;
	/* insert value to basic atom */

	SQUARE_TWO(single_atom_number, n_val_list);
	val_list=(int **)(malloc(sizeof(int *)*n_val_list));
	for(i=0; i<n_val_list; i++)
	{
		val_list[i]=A_I_L(l);
		memset(val_list[i], 0, (sizeof(int)*l));
	}/* end for */
	atomListMaker();
	/* compute complex value to list */
	i = single_atom_number;
	char *str_tmp=A_S(analyze_string_len);
	while(i<l)
	{	
		memset(str_tmp, 0, analyze_string_len);
		strcpy(str_tmp, object_symbol[i]);
		if(str_tmp[strlen(str_tmp)-1]!=';')
		{
			strcat(str_tmp, "@");
		}/* end if */
		SETACTIVESTR(str_tmp);		
		for(j=0; j<n_val_list; j++)
		{	
			val_list[j][i]=maths_logic_expression_vt(j);
			DEFAULT_S_POINT;
		}/* end for */
		i++;
	}/* end while */
	free(str_tmp);
	return;
}/* end verTableMaker */

int vtnext_token(void)
{	
	int index;
	memset(vttoken_buffer, 0, TOKENSIZE);
	while(1)
	{	
		SGETC(scaner_str, vtin_char);
		if((vtin_char==' ')||(vtin_char=='\t')||(vtin_char=='\r')||
			(vtin_char=='\n'))
		{
			continue;
		}/* end if */

		if(((vtin_char>='a')&&(vtin_char<='z'))||((vtin_char>='A')&&(vtin_char<='Z'))||
			((vtin_char>='0')&&(vtin_char<='9'))||(vtin_char=='_'))
		{
			index=0;
			do	
			{	
				vttoken_buffer[index]=vtin_char;
				index++;
				SGETC(scaner_str, vtin_char);
				
			}while(((vtin_char>='a')&&(vtin_char<='z'))||((vtin_char>='A')&&(vtin_char<='Z'))||
					((vtin_char>='0')&&(vtin_char<='9'))||(vtin_char=='_'));

			UNSGETC;
			id = getIdFromSrc(vttoken_buffer);
			id= id * -1;
			return id;
		}/* end if */

		switch(vtin_char)
		{
			case '~':
			{				
				return NOT;
			}break;
			case '|':
			{
				SGETC(scaner_str, vtin_char);
				if(vtin_char=='|')
				{
					return OR;
				}/* end if */
			}break;
			case '&':
			{
				SGETC(scaner_str, vtin_char);
				if(vtin_char=='&')
				{
					return AND;
				}/* end if */
			}break;
			case '^':
			{
				return XOR;
			}break;
			case '-':
			{
				SGETC(scaner_str, vtin_char);
				if(vtin_char=='>')
				{
					return IMP;
				}/* end if */
			}break;
			case '<':
			{
				SGETC(scaner_str, vtin_char);
				if(vtin_char=='-')
				{
					SGETC(scaner_str, vtin_char);	
					if(vtin_char=='>')
					{
						return EQV;
					}/* end if */
				}/* end if */
			}break;
			case '(':
			{
				return LPAREM;
			}break;
			case ')':
			{
				return RPAREM;
			}break;
			case ';':
			{
				return SEMICOLON;
			}break;
			case '@':
			{
				return ANYEND;
			}break;
		}/* end switch */
	}/* end while */
}/* end vtnext_token */

int  maths_logic_expression_vt(int c)
{
    	int value;
    	int i=0;
	int *ram;
	ram=A_I_L(token_ram_len);
    	memset(ram, END, (token_ram_len*sizeof(int)));
    	token=vtnext_token(); 
    	while(1)
    	{   
       		if((token==SEMICOLON)||(token==ANYEND))
       		{  
          		ram[i]=END;
          		break;
       		}/* end if */ 
       		else if(token==LPAREM)
       		{   
           		ram[i]=maths_logic_clause_vt(c);
           		i++;
       		}/* end else if */
       		else if((token>=NOT)&&(token<=IMP))
       		{   
           		ram[i]=token;
           		i++;
       		}/* end if */
       		else
       		{	
			token = token * -1;
           		ram[i]=val_list[c][token];
           		i++;			
       		}/* end else */
       		token=vtnext_token();
    	}/* end while */
    	COMPUTE_COMPLEX(ram,value);
	free(ram);
    	return value;	
}/* end maths_logic_expression_vt */

int maths_logic_clause_vt(int c)
{  
   	DBGPRT("Function: maths_logic_clause\r\n");
   	int value;
   	int *ram;
   	int i=0;
   	ram=A_I_L(token_ram_len);
   	memset(ram,SKIP,(token_ram_len*sizeof(int)));   
   	while(1)
   	{  
      		token=vtnext_token();
      		if(token==RPAREM)
      		{
         		break;
      		}/* end if */
      		else if(token==LPAREM)
      		{
         		ram[i]=maths_logic_clause_vt(c);
         		i++;
      		}/* end else if */
      		else if((token>=NOT)&&(token<=IMP))
      		{
         		ram[i]=token;
         		i++;
      		}/* end else if */
      		else
      		{	
			token = token * -1;
           		ram[i]=val_list[c][token];
           		i++;	        		
      		}/* end else */
   	}/* end while */
   	ram[i]=END;
   	COMPUTE_COMPLEX(ram,value);
   	free(ram);
   	DBGPRT("End maths_logic_clause\r\n");
   	return value;
}/* end maths_logic_clause_vt */

void kr_show_vertbl(char *s)
{	
	int n_valid_list;
	int i;
	int j;
	initMemoryTree();
	n_valid_list = analyzeLogicString(s);
	verTableMaker(n_valid_list);
	for(i=0;i<n_valid_list;i++)
	{
		printf("%s.\t",object_symbol[i]);
	}/* end for */

	printf("\r\n");

	for(i=0;i<n_val_list;i++)
	{
		for(j=0;j<n_valid_list;j++)
		{
			printf("%d\t", val_list[i][j]);
		}/* end for */
		printf("\r\n");
	}/* end for */
	for(i=0; i<n_val_list; i++)
	{
		if(val_list[i]!=NULL)free(val_list[i]);
	}/* end for */
	if(val_list!=NULL)free(val_list);
	freeMemoryTree();
	return;
}/* end kr_show_vertbl */

void kr_show_vertblex(char *s)
{
	int n_valid_list;
	int i;
	int j;
	initMemoryTree();
	n_valid_list = analyzeLogicString(s);
	verTableMaker(n_valid_list);

	for(j=0;j<n_valid_list;j++)
	{	
		printf("%s= ", object_symbol[j]);
		for(i=0;i<n_val_list;i++)
		{
			printf("%d ", val_list[i][j]);
		}/* end for */
		printf("\r\n");
	}/* end for */
	printf("\r\n");
	for(i=0; i<n_val_list; i++)
	{
		if(val_list[i]!=NULL)free(val_list[i]);
	}/* end for */
	if(val_list!=NULL)free(val_list);
	freeMemoryTree();
	return;	
}/* end kr_show_vertblex */

void kr_factorization(char *s)
{	
	int n_valid_list;
	int i;
	initMemoryTree();
	n_valid_list = analyzeLogicString(s);
	for(i=0;i<n_valid_list;i++)
	{
		printf("%s\r\n",object_symbol[i]);
	}/* end for */
	freeMemoryTree();
	return;
}/* end kr_factorization */



