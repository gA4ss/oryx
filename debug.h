//#define _DEBUG_LOGIX		1
#ifdef	_DEBUG_LOGIX
#define DBGPRT(D)		printf(D)
#define DBGPRT1(D,C)		printf(D,C)
#define DBGPRT2(D,R,I)		{\
				   int i=0;\
				   while(i<=I)\
				   {\
				      printf("R[%d]= %d\r\n",i,R[i]);\
				      i++;\
				   }\
				}
#else
#define DBGPRT(D)
#define DBGPRT1(D,C)
#define DBGPRT2(D,R,I)
#endif
