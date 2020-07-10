/* HEAD FILE */
#include "headfile.h"
/* function */
extern void handleArguments(int argc, char *argv[]);		/* cmdline.c */
/* main */
int main(int argc, char *argv[])
{  
    	handleArguments(argc,argv);
    	return 0;		 				/* SHUTDOWN_NORMAL define in error.c */
}/* end main */

