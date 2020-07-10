#include "token.h"
#include "debug.h"
int M,N,MID,COUNT;
#define GOSKIP(R)	        DBGPRT("Macro: GOSKIP\r\n");\
                                M=0;\
				N=0;\
                                MID=0;\
			        COUNT=0;\
                                while(R[M]!=END)\
                                {\
                                   M++;\
                                }\
				COUNT=M;\
				M=0;\
                                for(;M<COUNT;M++)\
                                {\
				    N=0;\
                                    for(;N<COUNT;N++)\
                                    {\
                                       if(R[N]==SKIP)\
                                       {\
                                          MID=R[N];\
                                          R[N]=R[N+1];\
                                          R[N+1]=MID;\
                                       }\
                                    }\
                                }\
				DBGPRT("End GOSKIP\r\n");
/*                            
#define HANDLE_EXP_NOT(R,I)	I=0;\
				while(R[I]!=END)\
				{\
				   if(R[I]==NOT)\
				   {\
				       if(R[I+1]=TRUE)\
				       {\
					  R[I+1]=FALSE;\
				       }\
				       else\
				       {\
					  R[I+1]=TRUE;\
				       }\
				       R[I]=SKIP;\
				   }\
				   I++;\
				}\
				GOSKIP(R);
*/						                                
#define HANDLE_EXP_NOT(R,I)	{\
				   DBGPRT("Macro: HANDLE_EXP_NOT\r\n");\
				   int count=1;\
				   I=0;\
				   while(R[I]!=END)\
				   {\
				      if(R[I]==NOT)\
				      {\
					 R[I]=SKIP;\
				         if(R[I+1]==NOT)\
				         {\
					    count++;\
					    I++;\
					    continue;\
				         }\
					 if((count % 2)==0)\
					 {\
					    R[I+1]=R[I+1];\
					 }\
					 else\
					 {\
					    if(R[I+1]==TRUE)\
					    {\
						R[I+1]=FALSE;\
					    }\
					    else\
				            {\
						R[I+1]=TRUE;\
					    }\
                                         }\
				      }\
				      I++;\
				   }\
				   GOSKIP(R);\
				   DBGPRT("End HANDLE_EXP_NOT\r\n");\
				}
                                
#define HANDLE_EXP_AND(R,I)	DBGPRT("Macro: HANDLE_EXP_AND\r\n");\
                                I=0;\
                                while(R[I]!=END)\
                                {\
                                   if(R[I]==AND)\
                                   {\
                                      R[I+1]=(R[I-1] & R[I+1]);\
                                      R[I]=SKIP;\
                                      R[I-1]=SKIP;\
                                   }\
                                   I++;\
                                }\
                                GOSKIP(R);\
                                DBGPRT("End HANDLE_EXP_AND\r\n");

#define HANDLE_EXP_OR(R,I)      DBGPRT("Macro: HANDLE_EXP_OR\r\n");\
				I=0;\
                                while(R[I]!=END)\
                                {\
                                   if(R[I]==OR)\
                                   {\
                                      R[I+1]=(R[I-1] | R[I+1]);\
                                      R[I]=SKIP;\
                                      R[I-1]=SKIP;\
                                   }\
                                   I++;\
                                }\
                                GOSKIP(R);\
				DBGPRT("End HANDLE_EXP_OR\r\n");

#define HANDLE_EXP_IMP(R,I) 	DBGPRT("Macro: HANDLE_EXP_IMP\r\n");\
				I=0;\
                                while(R[I]!=END)\
                              	{\
                                   if(R[I]==IMP)\
                                   {\
                                      if(R[I-1]==TRUE)\
				      {\
					 R[I-1]=FALSE;\
				      }\
				      else\
				      {\
					 R[I-1]=TRUE;\
				      }\
                                      R[I+1]=(R[I-1] | R[I+1]);\
                                      R[I]=SKIP;\
                                      R[I-1]=SKIP;\
                                   }\
                                   I++;\
                                }\
                                GOSKIP(R);\
				DBGPRT("End HANDLE_EXP_IMP\r\n");

#define HANDLE_EXP_XOR(R,I)     DBGPRT("Macro: HANDLE_EXP_XOR\r\n");\
				I=0;\
                                while(R[I]!=END)\
                               	{\
                                   if(R[I]==XOR)\
                                   {\
                                      R[I+1]=(R[I-1] ^ R[I+1]);\
                                      R[I]=SKIP;\
                                      R[I-1]=SKIP;\
                                   }\
                                   I++;\
                                }\
                                GOSKIP(R);\
				DBGPRT("End HANDLE_EXP_XOR\r\n");

#define HANDLE_EXP_EQV(R,I)	DBGPRT("Macro: HANDLE_EXP_EQV\r\n");\
				I=0;\
				while(R[I]!=END)\
				{\
				   if(R[I]==EQV)\
				   {\
				      if(((R[I-1]==TRUE)&&(R[I+1]==TRUE))||\
						((R[I-1]==FALSE)&&(R[I+1]==FALSE)))\
				      {\
				          R[I+1]=TRUE;\
				      }\
				      else\
				      {\
					  R[I+1]=FALSE;\
				      }\
				      R[I]=SKIP;\
				      R[I-1]=SKIP;\
				  }\
				  I++;\
				}\
				GOSKIP(R);\
				DBGPRT("End HANDLE_EXP_EQV\r\n");

#define COMPUTE_COMPLEX(R,V)	{\
				   DBGPRT("Macro: COMPUTE_COMPLEX\r\n");\
				   int i=0;\
				   HANDLE_EXP_NOT(R,i);\
				   HANDLE_EXP_AND(R,i);\
				   HANDLE_EXP_OR(R,i);\
				   HANDLE_EXP_XOR(R,i);\
				   HANDLE_EXP_EQV(R,i);\
				   HANDLE_EXP_IMP(R,i);\
				   V=R[0];\
			           DBGPRT("End COMPUTE_COMPLEX\r\n");\
      				}

