/* dual.c */
#include "headfile.h"
#include "config.h"
#include "debug.h"
#include "alloc.h"
#include "consequence_token.h"

extern int cenext_token(void);				/* consequence_token.c */
extern void setActiveAnalyzeSrc(char *s);		/* consequence_token.c */
extern void freeActiveAnalyzeSrc();			/* consequence_token.c */
extern void kr_show_dual(char *s);


extern char cetoken_buffer[TOKENSIZE];			/* consequence_token.c */
extern int analyze_string_len;				/* run.c */
char *analyze_string;
int  cetoken;

void kr_show_dual(char *s)
{	
	char *dual_string;
	int not_count=0;
	analyze_string=s;
	setActiveAnalyzeSrc(analyze_string);
	/* start analyze string */
	dual_string=A_S(analyze_string_len);
	memset(dual_string, 0, analyze_string_len);

	while((cetoken=cenext_token())!=SEMICOLON)
	{
		switch(cetoken)
		{
			case NOT:
			{	
				not_count++;
			}break;
			case AND:
			{
				strcat(dual_string, "||");
			}break;
			case OR:
			{
				strcat(dual_string, "&&");
			}break;
			case ID:
			{	
				if((not_count%2)==0)
				{
					strcat(dual_string, "~");
				}/* end if */
				strcat(dual_string, cetoken_buffer);
				not_count=0;
			}break;
			case LPAREM:
			{
				if((not_count%2)==0)
				{
					strcat(dual_string, "~");
				}/* end if */
				strcat(dual_string, "(");
				not_count=0;				
			}break;
			default:
			{					
				strcat(dual_string, cetoken_buffer);
			}
		}/* end switch */
	}/* end while */
	/* printf dual string */
	printf("%s", dual_string);
	freeActiveAnalyzeSrc();
	/* free dual string */
	free(dual_string);
	return;
}/* end kr_show_dual */

