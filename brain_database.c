/* brain_database.c */
#include "headfile.h"
#include "debug.h"
#include "config.h"
#include "alloc.h"
#include "consequence_token.h"


int formula[32]={0};

int formula_list[24][32]=
	{	
		{IDA},
		{IDA},
		{IDA},
		{IDA,OR,IDB},					/* A || B */
		{IDA,AND,IDB},					/* A && B */
		{LPAREM,IDA,OR,IDB,RPAREM,OR,IDC},		/* (A || B) || C */
		{LPAREM,IDA,AND,IDB,RPAREM,AND,IDC},		/* (A && B) && C */
		{IDA,OR,LPAREM,IDB,AND,IDC,RPAREM},		/* A || (B && C)*/
		{IDA,AND,LPAREM,IDB,OR,IDC,RPAREM},		/* A && (B || C) */
		{NOT,LPAREM,IDA,OR,IDB,RPAREM},			/* ~(A || B) */
		{NOT,LPAREM,IDA,AND,IDB,RPAREM},		/* ~(A && B) */
		{IDA,OR,LPAREM,IDA,AND,IDB,RPAREM},		/* A || (A && B) */	
		{IDA,AND,LPAREM,IDA,OR,IDB,RPAREM},		/* A && (A || B) */
		{IDA,OR,1},					/* A || 1 */
		{IDA,AND,0},					/* A && 0 */
		{IDA,OR,0},					/* A || 0 */
		{IDA,AND,1},					/* A && 1 */
		{IDA,OR,NOT,IDA},				/* A || ~A */
		{IDA,AND,NOT,IDA},				/* A && ~A */
		{IDA,IMP,IDB},					/* A -> B */		
		{IDA,EQV,IDB},					/* A <-> B */
		{IDA,IMP,IDB},					/* A -> B */
		{IDA,EQV,IDB},					/* A <-> B */
		{LPAREM,IDA,IMP,IDB,RPAREM,AND,LPAREM,IDA,IMP,NOT,IDB,RPAREM}
								/* (A -> B) && (A -> ~B) */
	};

int formula_len[24]=
	{
		1,1,1,3,3,7,7,7,7,6,6,7,7,3,3,3,3,4,4,3,3,3,3,12	
	};

int converse_formula_list[24][32]=
	{	
		{NOT,NOT,IDA},					/* ~ ~ A */
		{IDA,OR,IDA},					/* A || A */
		{IDA,AND,IDA},					/* A && A */
		{IDB,OR,IDA},					/* B || A */
		{IDB,AND,IDA},					/* B && A */
		{IDA,OR,LPAREM,IDB,OR,IDC,RPAREM},		/* A || (B || C ) */
		{IDA,AND,LPAREM,IDB,AND,IDC,RPAREM},		/* A && (B && C ) */
		{LPAREM,IDA,OR,IDB,RPAREM,AND,LPAREM,IDA,OR,IDC,RPRAEM},	
								/* (A || B) && (A || C) */
		{LPAREM,IDA,AND,IDB,RPAREM,OR,LPAREM,IDA,AND,IDC,RPRAEM},	
								/* (A && B) || (A && C) */
		{NOT,IDA,AND,NOT,IDB},				/* ~A && ~B */	
		{NOT,IDA,OR,NOT,IDB},				/* ~A || ~B */	
		{IDA},
		{IDA},
		{1},
		{0},
		{IDA},		
		{IDA},
		{1},
		{0},
		{NOT,IDA,OR,IDB},				/* ~A || B */
		{LPAREM,IDA,IMP,IDB,RPAREM,AND,LPAREM,IDB,IMP,IDA,RPAREM},
								/* (A -> B) && (B -> A) */
		{NOT,IDB,IMP,NOT,IDA},				/* ~B -> ~A */
		{NOT,IDA,EQV,NOT,IDB},				/* ~A -> ~B */
		{NOT,IDA}					/* ~A */
	};

int converse_formula_len[24]=
	{
		3,3,3,3,3,7,7,11,11,5,5,1,1,1,1,1,1,1,1,4,11,5,5,2
	};

extern int  findFormulaNode(int index, int n);
extern int  findConverseFormulaNode(int index, int n);

int findFormulaNode(int index, int n)
{
	int i=index;
	for(;i<24;i++)
	{	
		if(formula_len[i]!=n)
		{
			coutinue;
		}/* end if */
		if(memcmp(formula, formula_list[i], sizeof(int)*n)==0)
		{
			return i;
		}/* end if */
	}/* end for */
	return -1;
}/* end findFormulaNode */

int  findConverseFormulaNode(int index, int n)
{
	int i=index;
	for(;i<24;i++)
	{	
		if(converse_formula_len[i]!=n)
		{
			coutinue;
		}/* end if */
		if(memcmp(formula, converse_formula_list[i], sizeof(int)*n)==0)
		{
			return i;
		}/* end if */
	}/* end for */
	return -1;	
}/* end findConverseFormulaNode */

