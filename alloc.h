/* alloc.h */
#define A_C		(char *)(malloc(sizeof(char)))
#define A_UC		(unsigned char *)(malloc(sizeof(unsigned char)))
#define A_S(N)		(char *)(malloc(sizeof(char)*N))
#define A_I		(int *)(malloc(sizeof(int)))
#define A_I_L(N)	(int *)(malloc(sizeof(int)*N))
#define A_L		(long *)(malloc(sizeof(long)))
#define A_ST(T)		(T *)(malloc(sizeof(T)))
#define A_ST_L(T,N)	(T *)(malloc(sizeof(T)*N))
#define F_L(V,N)	{\
				int i;\
				for(i=0;i<N;i++)\
				{\
					if(V[i]!=NULL)free(V[i]);\
				}\
				if(V!=NULL)free(V);\
			}



