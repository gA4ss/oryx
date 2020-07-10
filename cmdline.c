/* cmdline.c */
#include "headfile.h"
#include "debug.h"
#include "config.h"
#include "cmdline.h"

/* function */
extern void handleArguments(int argc, char *argv[]);
void help(char *v, char *s);
void usage(char *v, char *s);

/* function */
extern void run(cmdline *cl);		/* run.c */
/* handleArguments */
void handleArguments(int argc, char *argv[])
{   
    	DBGPRT("Function: handleArguments\r\n");

    	cmdline cl={0}; 
    	cl.compute_ram_len=DEFRAMSIZE;
    	cl.analyze_string_len=STRINGSIZE;
	cl.symbol_atom_number=OBJECTLISTSIZE;
	cl.token_ram_len=ANALYZETOKENSIZE;
	cl.equfigure_list_len=EQUFIGURELISTSIZE;

    	strcpy(cl.filename,UNKNOWNAME);
    	if(argc<2)
    	{
       		usage(VERSION,argv[0]);
       		return;
    	}/* end if */
    	int i=1;
    	while(i<argc)
    	{ 
       		if(argv[i][0]=='-')
       		{
          		switch(argv[i][1])
          		{  
             			case 'f':
             			{
               				if(argc<3)
               				{
 						/* error */
                  				return;
               				}/* end if */
               				strcpy(cl.filename,argv[++i]);
             			}
             			break;
             			case 'h': 
             			{            
              	 			help(VERSION,argv[0]);
               				return;
             			}
             			break;
             			case 'c':
             			{
               				printf("console mode is not use now. ^_^\r\n");
               				return;
             			}
             			break;
             			case 'd':
             			{
               				printf("debug mode is not use now. *_*\r\n");
               				return;
             			}	
             			break;
             			case 's':
             			{
               				if(argv[i][2]=='l')
               				{
                  				cl.analyze_string_len=atoi(argv[++i]);
               				}/* end if */
               				else
               				{
                  				/* error */
               				}/* end else */
             			}
             			break;
             			case 'r':
             			{             
               				if(argv[i][2]=='s')
               				{
                  				cl.compute_ram_len=atoi(argv[++i]); 
               				}/* end if */
               				else
               				{                
                 				/* error */ 
               				}/* end else */
             			}break;
				case 'a':
				{
					if(argv[i][2]=='l')
					{
						cl.symbol_atom_number=atoi(argv[++i]);
					}/* end if */
					else
					{
						/* error */
					}/* end else */
				}break;
				case't':
				{
					if(argv[i][2]=='l')
					{
						cl.token_ram_len=atoi(argv[++i]);
					}/* end if */
					else
					{
						/* error */
					}/* end else */
				}break;
				case 'e':
				{
					if(argv[i][2]=='l')
					{
						cl.equfigure_list_len=atoi(argv[++i]);
					}/* end if */
					else
					{
						/* error */
					}/* end else */
				}break;
             			default:
             			{
               				/* error */
             			}/* end default */            
          		}/* end switch */
       		}/* end if */
       		else
       		{
          		/* error */
       		}/* end else */
       		i++;
    	}/* end while */
    	/* anything start now */
    	run(&cl);	
    	DBGPRT("End handleArguments\r\n");
    	return;
}/* end handleArguments */

void help(char *v,char *s)
{ 
  	printf("\t-=OryX Version %s=-\r\n",v);
  	printf("%s [Options]\r\n",s);
  	printf("   -f filename.\r\n"); 
  	printf("   -c console mode.\r\n");
  	printf("   -d enalbe debug mode.\r\n");
  	printf("   -rs xxx length for expression's run ram.\r\n");
  	printf("   -sl xxx length for expression's string.\r\n");
	printf("   -al xxx length for atom list.\r\n");
	printf("   -tl xxx length for analyze token ram.\r\n");
	printf("   -el xxx length for equivalence figure.\r\n");
  	printf("   -h show help.\r\n");
  	printf("<Author:logic.yan@gmail.com>.\r\n");
  	printf("\r\n");
  	return;
}/* end usage */

void usage(char *v,char *s)
{ 
  	printf("\r\n"); 
  	printf("WarCoder (R) 32-bit OryX Script Interpreter Version %s\r\n",v);
  	printf("usage:%s [option...]\r\n\r\n",s);
  	return;
}/* end title */

