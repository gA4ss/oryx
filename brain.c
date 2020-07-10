/* brain.c */
#include "headfile.h"
#include "debug.h"
#include "config.h"
#include "alloc.h"
#include "consequence_token.h"

/* data */
extern char cetoken_buffer[TOKENSIZE];			/* consequence_token.c */
extern char **object_symbol;				/* run.c */
extern int token_ram_len;				/* run.c */
extern int analyze_string_len;				/* run.c */
extern int symbol_atom_number;				/* run.c */

int  *token_ram=NULL;
char *analyze_string;
int  object_count=0;
int  cetoken;

/* function */
extern int cenext_token(void);				/* consequence_token.c */
extern void setActiveAnalyzeSrc(char *s);		/* consequence_token.c */
extern void freeActiveAnalyzeSrc();			/* consequence_token.c */
extern int  getTypeFromId(int id);			/* memory.c */
extern int  getIdFromSrc(char *s);			/* memory.c */
extern int  *getTokenRamFromId(int id);			/* memory.c */
extern int  getTypeFromSrc(char *s);			/* memory.c */
extern bool makeSureSrcBeRecorded(char *s);		/* memory.c */

/* local function */
extern int analyzeLogicString(char *s);
void makeAnalyzeTokenRing(void);
void runAnalyzeSystem(int *r);
void doMergerParem(int *r, int i, int n, int d);
int  mergerParem(int *r, int l);
void mergerParemMain(int *r);
void doMergerNot(int *r, int i, int l, int d);
void mergerNot(int *r, int j);
void mergerHandleNotInParem(int d);
void mergerNotMain(int *r);
void doMergerOpt(int *r, int i, int d);
void mergerOptInParem(int d, int op);
void mergerOpt(int *r, int j);
void mergerOptMain(int *r ,int op);
void changeTokenToStringAndAppend(char *s, int token);
void changeTokenRamToString(char *s,int *ram);
void changeStringToTokenRam(int *r, char *s, char c);
int  changeCharToToken(char c);

#define RECORD_ID_TO_SYMBOL(B)			strcpy(object_symbol[object_count],B);\
						object_count++;

#define RECORD_ID_TO_TREE(B,D,T,R)		addPageToMemory(B,D,T,R);\
						RECORD_ID_TO_SYMBOL(B);	

/* function */				
int analyzeLogicString(char *s)
{	
	analyze_string=s;
	object_count=0;
	setActiveAnalyzeSrc(analyze_string);

	makeAnalyzeTokenRing();

	runAnalyzeSystem(token_ram);

	freeActiveAnalyzeSrc();

	return object_count;	
}/* end analyzeLogicString */

void makeAnalyzeTokenRing(void)
{	
	int token_index=0;
	if(token_ram!=NULL)free(token_ram);
	token_ram=A_I_L(token_ram_len);
	memset(token_ram,0,sizeof(int)*token_ram_len);
	while((cetoken=cenext_token())!=SEMICOLON)
	{
		if(cetoken==ID)
		{
			if(!makeSureSrcBeRecorded(cetoken_buffer))
			{	
				token_ram[token_index]=object_count;
				RECORD_ID_TO_TREE(cetoken_buffer, object_count, ATOM, NULL);			
			}/* end if */
			else
			{
				token_ram[token_index]=getIdFromSrc(cetoken_buffer);
			}/* end else */
			token_index++;	
			continue;
		}/* end if */
		token_ram[token_index]=cetoken;
		token_index++;
	}/* end while */
	token_ram[token_index]=cetoken;		/* add SEMICOLON */
	token_index++;
	token_ram[token_index]=TOKENRAM_END;
	/* RECORD_ID_TO_TREE(analyze_string, object_count, PROJECT, token_ram); */
	return;
}/* end makeAnalyzeTokenRing */

void runAnalyzeSystem(int *r)
{
	mergerParemMain(r);
	mergerNotMain(r);
	mergerOptMain(r, AND);
	mergerOptMain(r, OR);
	mergerOptMain(r, XOR);
	mergerOptMain(r, EQV);
	mergerOptMain(r, IMP);
	return;
}/* end runAnalyzeSystem */

void doMergerParem(int *r, int i, int n, int d)
{
	int *tmp1=A_I_L(token_ram_len);
	int *tmp2=A_I_L(i);

	memset(tmp1, 0, (sizeof(int)*(token_ram_len - i - n)));
	memset(tmp2, 0, (sizeof(int)*i));
	memcpy(tmp1, r+i+n, (sizeof(int)*(token_ram_len - i - n)));	
	memcpy(tmp2, r, (sizeof(int)*i));	

	memset(r, 0, (token_ram_len*sizeof(int)));
	
	memcpy(r, tmp2, (sizeof(int)*i));

	r[i]=d;

	memcpy(r+i+1, tmp1, sizeof(int)*(token_ram_len - i - n));

	free(tmp1);
	free(tmp2);
	return;
}/* end doMergerParem */

int mergerParem(int *r, int l)
{
	int i=0;
	int *r_ptr=r+l;
	char *tmp=A_S(analyze_string_len);
	memset(tmp, 0, analyze_string_len);
	int *ram_tmp=A_I_L(token_ram_len);
	memset(ram_tmp , 0, token_ram_len);
	
	ram_tmp[i]=LPAREM;	/* ram_tmp[i] = r_ptr[i] */
	i++;
	
	while(r_ptr[i]!=RPAREM)
	{	
		if(r_ptr[i]==LPAREM)
		{
			ram_tmp[i]=mergerParem(r, l+i);			
		}/* end if */
		else
		{
			ram_tmp[i]=r_ptr[i];
		}/* end else */
		i++;
	}/* end while */
	ram_tmp[i]=RPAREM;
	i++;
	ram_tmp[i]=TOKENRAM_END;
	changeTokenRamToString(tmp, ram_tmp);

	doMergerParem(r, l, i, object_count);	

	int prev_id=object_count;

	if(!makeSureSrcBeRecorded(tmp))
	{
		RECORD_ID_TO_TREE(tmp, object_count, PAREM, ram_tmp);	
	}/* end if */

	/* analyze ram_tmp */
	mergerHandleNotInParem(prev_id);
	mergerOptInParem(prev_id, AND);
	mergerOptInParem(prev_id, OR);
	mergerOptInParem(prev_id, XOR);
	mergerOptInParem(prev_id, EQV);
	mergerOptInParem(prev_id, IMP);
	/* end analyze ram_tmp */
	free(ram_tmp);
	free(tmp);
	return prev_id;
}/* end mergerParem */

void mergerParemMain(int *r)
{
	int i=0;
	while(r[i]!=SEMICOLON)
	{
		if(r[i]==LPAREM)
		{	
			mergerParem(r, i);
		}/* end if */
		i++;
	}/* end while */
	return;
}/* end mergerParemMain */

void doMergerNot(int *r, int i, int l, int d)
{
	int *tmp1;
	int *tmp2;
	tmp1=A_I_L(token_ram_len);
	tmp2=A_I_L(token_ram_len);
	memset(tmp1,0,sizeof(int)*token_ram_len);
	memset(tmp2,0,sizeof(int)*token_ram_len);
	
	memcpy(tmp1,r,i*sizeof(int));
	memcpy(tmp2, r+i+l, (token_ram_len-i-l)*sizeof(int));
	memset(r,0,sizeof(int)*token_ram_len);
	
	memcpy(r, tmp1, i*sizeof(int));
	r[i]=d;
	memcpy(r+i+1, tmp2, (token_ram_len-i-l)*sizeof(int));

	free(tmp1);
	free(tmp2);
	return;
}/* end doMergerNot */

void mergerNot(int *r, int j)
{
	int i=0;
	char *tmp=A_S(analyze_string_len);
	memset(tmp, 0, analyze_string_len);
	int *ram_tmp=A_I_L(token_ram_len);	
	int *ram_ptr;

	ram_ptr=r+j;	

	while(ram_ptr[i]==NOT)
	{
		strcat(tmp,"~");
		ram_tmp[i]=NOT;
		i++;
	}/* end while */

	strcat(tmp, object_symbol[ram_ptr[i]]);	
	ram_tmp[i]=ram_ptr[i];
	ram_tmp[i+1]=TOKENRAM_END;

	int type=getTypeFromId(ram_tmp[i]);
	int insert_id;
	if(type==ATOM)
	{	
		if(!makeSureSrcBeRecorded(tmp))
		{	
			doMergerNot(r, j, i+1, object_count);
			RECORD_ID_TO_TREE(tmp, object_count, NOT_ATOM, ram_tmp);
		}/* end if */
		else
		{
			insert_id=getIdFromSrc(tmp);
			doMergerNot(r, j, i+1, insert_id);
		}/* end else */
	}/* end if */
	else 	/* type == PAREM */
	{	
		if(!makeSureSrcBeRecorded(tmp))
		{	
			doMergerNot(r, j, i+1, object_count);
			RECORD_ID_TO_TREE(tmp, object_count, NOT_PAREM, ram_tmp);
		}/* end if */
		else
		{
			insert_id=getIdFromSrc(tmp);
			doMergerNot(r, j, i+1, insert_id);
		}/* end else */
		mergerHandleNotInParem(ram_tmp[i]);
	}/* end else */

	free(ram_tmp);
	free(tmp);
	return;
}/* end mergerNot */

void mergerHandleNotInParem(int d)
{	
	int *ram_token;
	int i=0;

	ram_token=getTokenRamFromId(d);

	while(ram_token[i]!=RPAREM)
	{
		if(ram_token[i]==NOT)
		{
			mergerNot(ram_token, i);
		}/* end if */
		i++;
	}/* end while */
	return;
}/* end mergerHandleNot */

void mergerNotMain(int *r)
{	
	int i=0;
	while(r[i]!=SEMICOLON)
	{
		if(r[i]==NOT)
		{	
			mergerNot(r, i);
		}/* end if */
		i++;
	}/* end while */
	return;
}/* end mergerNotMain */

void doMergerOpt(int *r, int i, int d)
{
	int *tmp1=A_I_L(token_ram_len);
	memset(tmp1, 0, sizeof(int)*token_ram_len);
	
	memcpy(tmp1, r+i+3, (token_ram_len - i - 3)*sizeof(int));
	r[i]=d;
	memcpy(r+i+1, tmp1, (token_ram_len - i - 3)*sizeof(int));
	return;
}/* end doMergerOpt */

void mergerOptInParem(int d, int op)
{	
	int *token_ram_tmp;
	int i=0;
	int type=getTypeFromId(d);
	token_ram_tmp=getTokenRamFromId(d);
	if(type==NOT_PAREM)
	{
		while(token_ram_tmp[i]==NOT)
		{
			i++;
		}/* end while */
		token_ram_tmp=getTokenRamFromId(token_ram_tmp[i]);
	}/* end if */
	i=0;
	while(token_ram_tmp[i]!=RPAREM)
	{
		if(token_ram_tmp[i]==op)
		{
			mergerOpt(token_ram_tmp, i);
			i--;
		}/* end if */
		i++;
	}/* end while */
	return;
}/* end mergerOptInParem */

void mergerOpt(int *r, int j)
{	
	int *ram_tmp=A_I_L(token_ram_len);
	memset(ram_tmp,0,token_ram_len*sizeof(int));
	char *tmp=A_S(analyze_string_len);
	memset(tmp, 0, sizeof(char)*analyze_string_len);
	int p = *(r+j-1);
	int q = *(r+j+1);
	
	strcat(tmp, object_symbol[p]);
	changeTokenToStringAndAppend(tmp, r[j]);
	strcat(tmp, object_symbol[q]);
	
	ram_tmp[0]=p;
	ram_tmp[1]=r[j];
	ram_tmp[2]=q;
	ram_tmp[3]=TOKENRAM_END;

	doMergerOpt(r, j-1, object_count);
	if(!makeSureSrcBeRecorded(tmp))
	{
		RECORD_ID_TO_TREE(tmp, object_count, OBJECT, ram_tmp);
	}/* end if */
	int type=getTypeFromId(p);
	if((type==PAREM)||(type==NOT_PAREM))
	{
		mergerOptInParem(p, ram_tmp[1]);
	}/* end if */

	type=getTypeFromId(q);
	if((type==PAREM)||(type==NOT_PAREM))
	{
		mergerOptInParem(q, ram_tmp[1]);
	}/* end if */
	
	free(ram_tmp);
	free(tmp);
	return;
}/* end mergerOpt */

void mergerOptMain(int *r ,int op)
{
	int i=0;
	while(r[i]!=SEMICOLON)
	{
		if(r[i]==op)
		{
			mergerOpt(r, i);
			i--;
		}/* end if */
		i++;
	}/* end while */
	return;
}/* end mergerOpt*/


void changeTokenToStringAndAppend(char *s, int token)
{	
	switch(token)
	{
		case NOT:
		{
			strcat(s,"~");
		}break;
		case AND:
		{	
			strcat(s,"&&");
		}break;
		case OR:
		{	
			strcat(s,"||");	
		}break;
		case XOR:
		{	
			strcat(s,"^");	
		}break;
		case EQV:
		{		
			strcat(s,"<->");	
		}break;
		case IMP:
		{		
			strcat(s,"->");
		}break;
		case LPAREM:
		{	
			strcat(s,"(");	
		}break;
		case RPAREM:
		{	
			strcat(s,")");	
		}break;
		case SEMICOLON:
		{
			strcat(s,";");	
			return;
		}break;
		default:	/* ID */
		{					
			strcat(s, object_symbol[token]);
		}
	}/* end switch */
	return;
}/* end changeTokenToStringAndAppend */

void changeTokenRamToString(char *s,int *ram)
{
	int i=0;

	while(ram[i]!=TOKENRAM_END)
	{
		switch(ram[i])
		{	
			case SEMICOLON:
			{
				strcat(s,";");
				return;
			}break;
			case NOT:
			{
				strcat(s,"~");
			}break;
			case AND:
			{	
				strcat(s,"&&");	
			}break;
			case OR:
			{	
				strcat(s,"||");	
			}break;
			case XOR:
			{	
				strcat(s,"^");	
			}break;
			case EQV:
			{		
				strcat(s,"<->");	
			}break;
			case IMP:
			{		
				strcat(s,"->");
			}break;
			case LPAREM:
			{	
				strcat(s,"(");	
			}break;
			case RPAREM:
			{	
				strcat(s,")");	
			}break;
			default:	/* ID */
			{					
				strcat(s,object_symbol[ram[i]]);
			}
		}/* end switch */
		i++;
	}/* end while */
	return;
}/* end changeTokenRamToString */

void changeStringToTokenRam(int *r, char *s, char c)
{
	int i=0;
	int j=0;
	int m=0;
	char *id_token;

	id_token=A_S(analyze_string_len);
	memset(id_token, 0, analyze_string_len);

	while(s[i]!=c)
	{	
		switch(s[i])
		{	
			case ';':
			{
				r[j]=SEMICOLON;
				return;
			}break;
			case '~':
			{
				r[j]=NOT;
			}break;
			case '|':
			{
				i++;
				if(s[i]=='|')
				{
					r[j]=OR;
				}/* end if */
			}break;
			case '&':
			{
				i++;
				if(s[i]=='&')
				{
					r[j]=AND;
				}/* end if */
			}break;
			case '-':
			{
				i++;
				if(s[i]=='>')
				{
					r[j]=IMP;
				}/* end if */
			}break;
			case '^':
			{
				r[j]=XOR;
			}break;
			case '<':
			{
				i++;
				if(s[i]=='-')
				{	
					i++;
					if(s[i]=='>')
					{
						r[j]=EQV;
					}/* end if */
				}/* end if */
			}break;
			case '(':
			{
				r[j]=LPAREM;
			}break;
			case ')':
			{
				r[j]=RPAREM;
			}break;
			default:
			{	
				while(((s[i]>='a')&&(s[i]<='z'))||((s[i]>='A')&&(s[i]<='Z'))||
						((s[i]>='0')&&(s[i]<='9'))||(s[i]=='_'))
				{
					id_token[m]=s[i];
					m++;
					i++;
				}/* end while */
				r[j]=getIdFromSrc(id_token);
			}
		}/* end switch */
		i++; 
		j++;
	}/* end while */
	r[j]=changeCharToToken(c);
	free(id_token);
	return;
}/* end changeStringToTokenRam */

int  changeCharToToken(char c)				/* just for opt char */
{
	switch(c)
	{
		case '~':
		{
			return NOT;
		}break;
		case '&':
		{
			return AND;
		}break;
		case '|':
		{
			return OR;
		}break;
		case '-':
		{
			return IMP;
		}break;
		case '^':
		{
			return XOR;
		}break;
		case '(':
		{
			return LPAREM;
		}break;
		case ')':
		{
			return RPAREM;
		}break;
		case '<':
		{
			return EQV;
		}break;
		case ';':
		{
			return SEMICOLON;
		}break;
	}/* end switch */
}/* end changeCharToToken */



