/* consequence_token.c */
#include "headfile.h"
#include "config.h"
#include "debug.h"
#include "alloc.h"
#include "consequence_token.h"

extern int cenext_token(void);
extern void setActiveAnalyzeSrc(char *s);
extern void freeActiveAnalyzeSrc();

int ce_scaner(void);

char cein_char;
char cetoken_buffer[TOKENSIZE]={0};
char *expression_analyze;

extern int analyze_string_len;			/* run.c */

int	i=-1;		/* index of string that be analyzed */
#define SGETC(S,C)	i++;\
			C=S[i];

int cenext_token(void)
{
	return ce_scaner();
}/* end cenext_token */

void setActiveAnalyzeSrc(char *s)
{	
	i=-1;
	expression_analyze=A_S(analyze_string_len);
	memset(expression_analyze, 0, analyze_string_len);
	strcpy(expression_analyze, s);
	return;
}/* end setActiveAnalyeSrc*/

void freeActiveAnalyzeSrc()
{
	free(expression_analyze);
	return;
}/* end freeActiveAnalyzeSrc */

int ce_scaner(void)
{
	int  index;

	SGETC(expression_analyze,cein_char);
	memset(cetoken_buffer,0,TOKENSIZE);
	switch(cein_char)
	{	
		case ';':
		{	
			return SEMICOLON;
		}break;
		case '~':
		{	
			cetoken_buffer[0]='~';
			return NOT;
		}break;
		case '&':
		{
			SGETC(expression_analyze, cein_char);
			if(cein_char=='&')
			{		
				cetoken_buffer[0]='&';
				cetoken_buffer[1]='&';
				return AND;
			}/* end if */
		}break;
		case '|':
		{
			SGETC(expression_analyze,cein_char);
			if(cein_char=='|')
			{
				cetoken_buffer[0]='|';
				cetoken_buffer[1]='|';
				return OR;
			}/* end if */
		}break;
		case '^':
		{	
			cetoken_buffer[0]='^';
			return XOR;
		}break;
		case '<':
		{
			SGETC(expression_analyze,cein_char);
			if(cein_char=='-')
			{
				SGETC(expression_analyze,cein_char);
				if(cein_char=='>')
				{	
					cetoken_buffer[0]='<';
					cetoken_buffer[1]='-';
					cetoken_buffer[2]='>';
					return EQV;
				}/* end if */
			}/* end if */
			else if(cein_char=='=')
			{
				SGETC(expression_analyze,cein_char);
				if(cein_char=='>')
				{
					cetoken_buffer[0]='<';
					cetoken_buffer[1]='=';
					cetoken_buffer[2]='>';
					return EQP;
				}/* end if */
			}/* end else if */
		}break;
		case '-':
		{
			SGETC(expression_analyze,cein_char);
			if(cein_char=='>')
			{
				cetoken_buffer[0]='-';
				cetoken_buffer[1]='>';
				return IMP;
			}/* end if */
		}break;
		case '(':
		{	
			cetoken_buffer[0]='(';
			return LPAREM;
		}break;
		case ')':
		{	
			cetoken_buffer[0]=')';
			return RPAREM;
		}break;
		case '$':
		{
			index=0;
			SGETC(expression_analyze,cein_char);
			while(1)
			{
				if(((cein_char>='a')&&(cein_char<='z'))||
					((cein_char>='A')&&(cein_char<='Z'))||
						((cein_char>='0')&&(cein_char<='9'))||
							(cein_char=='_'))
				{
					cetoken_buffer[index]=cein_char;
					index++;
					SGETC(expression_analyze,cein_char);
				}/* end if */
				else
				{
					break;
				}/* end else */
			}/* end while */
			i--;
			return ID;
		}break;
	}/* end switch */
	return SEMICOLON;	
}/* end ce_scaner */
