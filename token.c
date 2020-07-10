/* token.c */
#include "debug.h"
#include "headfile.h"
#include "config.h"
#include "token.h"
/* Data */
extern char *expression_buffer;			/* syntax.c */
extern char *list_buffer;			/* syntax.c */
int  l_c=0;
extern int analyze_string_len;			/* syntax.c */
char token_buffer[TOKENSIZE]={0};
extern unsigned long line_count;		/* error.c */
/* Token Macro */
FILE *fp;
/* function */
extern int  getNodeType(char *s);		/* database.c */

extern void openLgxFile(char *s);
extern void closeLgxFile(char *s);
extern int next_token(void);

extern void lexicalError(char *s);		/* error.c */	
extern void fatal_lexicalError(char *s);	/* error.c */
extern void fatal_cnfof(char *s);		/* error.c */

int scaner(void);

/*==============================*/
#define	GOSPACE(C,FP)		while((C=='\r')||(C=='\n')||(C=='\t')||\
					(C==' '))\
				{\
					if(C=='\n')line_count++;\
			    		C=fgetc(FP);\
				}\
				ungetc(C,FP);

bool ifspace=false;
#define ISSPACE(C)		if((C=='\r')||(C=='\n')||(C=='\t')||\
					(C==' '))\
				{\
					if(C=='\n')line_count++;\
			   	 	ifspace=true;\
				}\
				else\
				{\
			    		ifspace=false;\
				}

bool ifvalidchar=false;
#define ISVALIDCHAR(C)		if(((C>='a')&&(C<='z'))||(C=='_')||\
					((C>='A')&&(C<='Z'))||\
						((C>='0')&&(C<='9')))\
				{\
			    		ifvalidchar=true;\
				}\
				else\
				{\
			    		ifvalidchar=false;\
				}

#define R_E(C)			list_buffer[l_c]=C;\
				l_c++;	

int scaner(void)		
{  
   	DBGPRT("Function: scaner\r\n");
	char in_char;
   	int index;
   	if(feof(fp))
   	{   
      		DBGPRT("scaner: return SCANEOF\r\n");
      		return SCANEOF;
   	}/* end if */
   	while(1)
   	{
      		in_char=fgetc(fp);
      		if(in_char==EOF)
      		{
         		DBGPRT("scaner: return SCANEOF\r\n");
	 		return SCANEOF;
      		}/* end if */
      		ISSPACE(in_char);
      		if(ifspace)
      		{ 
			continue;
      		}/* end if */
      		memset(token_buffer,0,TOKENSIZE);
      		switch(in_char)
      		{
			case '~':
			{  	
				R_E(in_char);
           			DBGPRT("scaner: return NOT\r\n");
	   			return NOT;
			}break;
			case '&':
			{	
				R_E(in_char);
	   			in_char=fgetc(fp);
				R_E(in_char);
	   			if(in_char=='&')
	   			{   
               				DBGPRT("scaner: return AND\r\n");
	       				return AND;
	   			}/* end if */
	   			else
	  	 		{
	       				lexicalError("&& lose & char");
	       				return AND;
	   			}/* end else */
			}break;
			case '|':
			{	
				R_E(in_char);
	   			in_char=fgetc(fp);
				R_E(in_char);
	   			if(in_char=='|')
	   			{   
                			DBGPRT("scaner: return OR\r\n");
					return OR;
	   			}/* end if */
	   			else
	   			{
					lexicalError("|| lose | char");
					return OR;
           			}/* end else if */
			}break;
			case '^':
			{  	
				R_E(in_char);
           			DBGPRT("scaner: return XOR\r\n");
	   			return XOR;
			}break;
			case '<':
			{	
				R_E(in_char);
	   			in_char=fgetc(fp);
				R_E(in_char);
	   			if(in_char=='-')
	   			{
	      				in_char=fgetc(fp);
					R_E(in_char);
	      				if(in_char=='>')
	      				{
                 				DBGPRT("scaner: return EQV\r\n");
		 				return EQV;
	      				}/* end if */
	      				else
	      				{
		 				lexicalError("<-> lose >");
		 				return EQV;
	      				}/* end else */
	   			}/* end if */
           			else if(in_char=='=')
	   			{
	      				in_char=fgetc(fp);
					R_E(in_char);
              				if(in_char=='>')
	      				{
		 				DBGPRT("scaner: return EQP\r\n");
		 				return EQP;
              				}/* end if */
              				else
	      				{
                 				lexicalError("<=> lose >");
		 				return EQP;
              				}/* end else */
           			}/* end else if */
	   			else
	   			{
	      				lexicalError("<-> or <=> lose >");
	   			}/* end else */
			}break;
			case '-':
			{	
				R_E(in_char);
	   			in_char=fgetc(fp);
				R_E(in_char);
	   			if(in_char=='>')
	   			{   
               				DBGPRT("scaner: return IMP\r\n");
	       				return IMP;
	   			}/* end if */
	   			else
	   			{
	       				lexicalError("-> lose >");
	       				return IMP;
	   			}/* end else */
			}break;
			case '#':
			{
	   			in_char=fgetc(fp);
	   			if(in_char=='>')
	   			{    
                			DBGPRT("scaner: return PREAND\r\n");
					return PREAND;
	   			}/* end if */
	   			else
	   			{
					lexicalError("#> lose >");
					return PREAND;
	   			}/* end else */
			}break;
			case '{':
			{  
  	   			DBGPRT("scaner: return BLPAREM\r\n");
	   			return BLPAREM;
			}break;
			case '}':
			{  
           			DBGPRT("scaner: return BRPAREM\r\n");
	   			return BRPAREM;
			}break;
			case '(':
			{  
				R_E(in_char);
           			DBGPRT("scaner: return LPAREM\r\n");
	   			return LPAREM;
			}break;
			case ')':
			{
				R_E(in_char);
           			DBGPRT("scaner: return RPAREM\r\n");
	   			return RPAREM;
			}break;
			case ';':
			{  
				R_E(in_char);
           			DBGPRT("scaner: return SEMICOLON\r\n");
				memset(expression_buffer,0,analyze_string_len);
				strcpy(expression_buffer,list_buffer);
				memset(list_buffer,0,analyze_string_len);
				l_c=0;
	   			return SEMICOLON;
			}break;
			case '=':
			{  	
				R_E(in_char);
           			DBGPRT("scaner: return ASSIGNOP\r\n");
	   			return ASSIGNOP;
			}break;
			case ',':
			{ 
           			DBGPRT("scaner: return COMMA\r\n");
	   			return COMMA;
			}break;
			case '$':
			{	
				R_E(in_char);
	   			index=0;
	   			in_char=fgetc(fp);
	   			while(1)
	   			{
	      				ISSPACE(in_char);
	      				ISVALIDCHAR(in_char);
	      				if((ifspace==false)&&(ifvalidchar==true))
	      				{	
						R_E(in_char);
		 				token_buffer[index]=in_char;
		 				index++;
		 				in_char=fgetc(fp);
	      				}/* end if */
	      				else
              				{
						break;
              				}/* end else */
	   			}/* end while */ 
	   			ungetc(in_char,fp);
           			DBGPRT("scaner: reture VARIABLE\r\n");
           			DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);
	   			return 	getNodeType(token_buffer);
			}break;
			case 'A':
			{
	   			in_char=fgetc(fp);
           			token_buffer[0]=in_char;
           			in_char=fgetc(fp);
	   			token_buffer[1]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[2]=in_char;
	   			DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);
           			if(strncmp(token_buffer,"TOM",3)==0)
	   			{	
  					memset(token_buffer,0,TOKENSIZE);
					in_char=fgetc(fp);		        
                			GOSPACE(in_char,fp);
					in_char=fgetc(fp); 
					if(in_char!='$')
					{
		   				lexicalError("variable lose $ title");
					}/* end if */
					in_char=fgetc(fp);
                			if(((in_char>='a')&&(in_char<='z'))||\
                        			((in_char>='A')&&(in_char<='Z'))||\
                              				(in_char=='_'))
					{
		    				index=0;
	            				while(1)
                    				{
                        				ISSPACE(in_char);
                        				ISVALIDCHAR(in_char);
                        				if((ifspace==false)&&(ifvalidchar==true))
                        				{	
                            					token_buffer[index]=in_char;
                            					index++;
                            					in_char=fgetc(fp);
                        				}/* end if */
                        				else
							{
                           					break;
							}/* end else */
                    				}/* end while */
                    				ungetc(in_char,fp);
                    				DBGPRT("scaner: return ATOMTYPE\r\n");
                    				DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);
		    				return ATOMTYPE;
                			}/* end if */
					else
					{
		    				lexicalError("invalid char on variable");
					}/* end else */
	   			}/* end if */
   	   			else
	  	 		{
					fatal_lexicalError("invalid variable type");
	   			}/* end else */
        		}break;
			case 'C':
			{
	   			in_char=fgetc(fp);
	   			token_buffer[0]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[1]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[2]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[3]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[4]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[5]=in_char;
	   			DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);
           			if(strncmp(token_buffer,"OMPLEX",6)==0)
           			{
	       				memset(token_buffer,0,TOKENSIZE);
               				in_char=fgetc(fp);
	       				GOSPACE(in_char,fp);
	       				in_char=fgetc(fp);
               				if(in_char!='$')
	       				{
                   				lexicalError("variable lose $ title");
	       				}/* end if */
               				in_char=fgetc(fp);
               				if(((in_char>='a')&&(in_char<='z'))||\
                       				((in_char>='A')&&(in_char<='Z'))||\
                              				(in_char=='_'))
               				{
                    				index=0;
                    				while(1)
                    				{
                       					ISSPACE(in_char);
                       					ISVALIDCHAR(in_char);
                       					if((ifspace==false)&&(ifvalidchar==true))
                       					{
                           					token_buffer[index]=in_char;
                           					index++;
                           					in_char=fgetc(fp);
                       					}
                       					else
		       					{
                          					break;
		       					}/* end else */
                    				}/* end while */
		    				DBGPRT("scaner: return COMPLEXTYPE\r\n");
		    				DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);                  
                    				ungetc(in_char,fp);
		    				return COMPLEXTYPE;
               				}/* end if */
               				else
	       				{
		    				lexicalError("invalid char on variable");
	       				}/* end if */
	   			}/* end if */
           			else
	   			{
              				fatal_lexicalError("invalid variable type");
	   			}/* end else */
        		}break;
			case 'S':
			{
           			in_char=fgetc(fp);
           			token_buffer[0]=in_char;
	   			in_char=fgetc(fp);
           			token_buffer[1]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[2]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[3]=in_char;
	   			in_char=fgetc(fp);
           			token_buffer[4]=in_char;
           			DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);
	   			if(strncmp(token_buffer,"TRING",5)==0)
           			{
	       				memset(token_buffer,0,TOKENSIZE);
               				in_char=fgetc(fp);
               				GOSPACE(in_char,fp);
	       				in_char=fgetc(fp);
               				if(in_char!='$')
	       				{
                  				lexicalError("variable lose $ title");
	       				}/* end if */
               				in_char=fgetc(fp);
               				if(((in_char>='a')&&(in_char<='z'))||\
                       				((in_char>='A')&&(in_char<='Z'))||\
                               				(in_char=='_'))
	       				{
                   				index=0;
                   				while(1)
                   				{
                      					ISSPACE(in_char);
                      					ISVALIDCHAR(in_char);
                      					if((ifspace==false)&&(ifvalidchar==true))
                      					{
                           					token_buffer[index]=in_char;
                           					index++;
                           					in_char=fgetc(fp);
                      					}/* end if */
                      					else
		      					{
                           					break;
		      					}/* end else */
                   				}/* end while */
                   				DBGPRT("scaner: return STRINGTYPE\r\n");
                   				DBGPRT1("scaner: token_buffer = %s\r\n",token_buffer);
                   				ungetc(in_char,fp);
		   				return STRINGTYPE;
               				}/* end if */
	       				else
	       				{
		   				lexicalError("invalid char on variable");
	       				}/* end else */
           			}/* end if */
           			else
	   			{
              				fatal_lexicalError("invalid variable type");
	   			}/* end else */
        		}break;
			case 'P':
			{
	   			in_char=fgetc(fp);
	   			token_buffer[0]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[1]=in_char;
	   			in_char=fgetc(fp);
	   			token_buffer[2]=in_char;
           			DBGPRT1("scaner: token_buffer = %s\r\n",token_buffer);
	   			if(strncmp(token_buffer,"RED",3))
	   			{
					memset(token_buffer,0,TOKENSIZE);
	        			in_char=fgetc(fp);
	        			GOSPACE(in_char,fp);
					in_char=fgetc(fp);
					if(in_char!='$')
					{
		    				lexicalError("variable lose $ title");
					}/* end if */
					in_char=fgetc(fp);
					if(((in_char>='a')&&(in_char<='z'))||\
						((in_char>='A')&&(in_char<='Z'))||\
							(in_char=='_'))
					{
		     				index=0;
		     				while(1)
		     				{
							ISSPACE(in_char);
							ISVALIDCHAR(in_char);
							if((ifspace==false)&&(ifvalidchar==true))
							{
			     					token_buffer[index]=in_char;
			     					index++;
			     					in_char=fgetc(fp);
							}/* end if */
							else
							{
			      					break;
							}/* end else */
		     				}
		     				ungetc(in_char,fp);
                     				DBGPRT("scaner: return PREDTYPE\r\n");
		     				DBGPRT1("scaner: token_buffer = %s\r\n",token_buffer);
		     				return PREDICATETYPE;
					}
					else
					{
		     				lexicalError("invalid char on variable");
					}/* end else */
	   			}/* end if */
	   			else
	   			{
					fatal_lexicalError("invalid variable type");
	   			}/* end else */
			}break;
			case '.':
			{
	   			in_char=fgetc(fp);
	   			ISVALIDCHAR(in_char);
	   			if(ifvalidchar)
	   			{/* handle procedure name */
                			index=0;
                			while(1)
                			{  
                   				ISVALIDCHAR(in_char);
                   				if(!ifvalidchar)
                   				{
                      					break;
                   				}/* end if */
		   				token_buffer[index]=in_char;
		   				index++;
		   				in_char=fgetc(fp);
                			}/* end while */
					ungetc(in_char,fp);
                			DBGPRT("scaner: return PROCEDURETYPE\r\n");
					DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);
					return PROCEDURETYPE;
	   			}/* end if */
	   			else if(in_char=='?')
	   			{
					index=0;
					while(1)
					{  
                   				in_char=fgetc(fp);
		   				ISVALIDCHAR(in_char);
		   				if(ifvalidchar)
		   				{
	 	       					token_buffer[index]=in_char;
		       					index++;
		   				}/* end if */
		   				else
		   				{
		      					break;
		   				}/* end else */
	   				}/* end while  */
                			DBGPRT("scaner: return LOGICLABEL\r\n");
                			DBGPRT1("scaner: token_buffer= %s\r\n",token_buffer);
	  				ungetc(in_char,fp);
	   				return LOGICLABEL;
           			}/* end else if */
	   			else
	   			{
					lexicalError("invalid function name");
					return PROCEDURETYPE;
	   			}/* end else */
			}break;
        		case 'T':
			case '1':
  			{   
				DBGPRT("scaner: return TRUEVAL\r\n");
				return TRUEVAL;
			}break;
			case 'F':
			case '0':
			{  
           			DBGPRT("scaner: return FALSEVAL\r\n");
	   			return FALSEVAL;
			}break;
			case '"':
			{
  	   			index=0;
	   			in_char=fgetc(fp);
	   			while(in_char!='"')
	   			{
					token_buffer[index]=in_char;
					index++;
					in_char=fgetc(fp);
	   			}/* end while */
           			DBGPRT("scaner: return STRINGVAL\r\n");
	   			DBGPRT1("token_buffer= %s\r\n",token_buffer);
	   			return STRINGVAL;
			}break;
			case '/':
			{
	   			in_char=fgetc(fp);
	   			if(in_char=='/')
	   			{
					while((in_char=fgetc(fp))!='\n')
					{
		    				continue;
					}
	   			}/* end if */
           			else if(in_char=='*')
           			{
                			while(1)
                			{
                   				in_char=fgetc(fp);
                   				if(in_char=='*')
                   				{
                      					in_char=fgetc(fp);
                      					if(in_char=='/')
                      					{
                         					break;
                      					}/* end if */
                   				}/* end if */
                			}/* end while */
           			}/* end else if */
			}break;
      		}/* end switch */
   	}/* end while */
   	return SCANEOF;
}/* end scaner */

void openLgxFile(char *s)
{
  	fp=fopen(s,"rb");
  	if(fp==NULL)
  	{  
    		/* error */
		fatal_cnfof(s);
  	}/* end if */
  	return;
}/* end initToken */

int next_token(void)
{	
   	return scaner();
}/* end next_token */

void closeLgxFile(char *s)
{
   	fclose(fp);
   	return;
}/* end closeLgxFile */



