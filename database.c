/* database.c */
#include "headfile.h"
#include "debug.h"
#include "alloc.h"
#include "token.h"
#include "maths.h"
#include "config.h"

typedef struct hashtbl_types
{
   	bool empty;
   	char src[TOKENSIZE];
   	int  type;
   	int  value;
	char *str;
	char *list;
   	struct hashtbl_types *left;
   	struct hashtbl_types *right;
}hashtbl;

/* Data */
hashtbl hashtree[PRIME];
extern int analyze_string_len;			
/* Function */
extern void addNodeToTree(char *s, int t);
extern int  getNodeType(char *s);
extern void putStringToNode(char *s, char *str);
extern void putIntValToNode(char *s, int v);
extern char *getStringFromNode(char *s);
extern int  getIntValFromNode(char *s);
extern void putExpressionToNode(char *s, char *e);
extern char *getExpressionStringFromNode(char *s);
extern void initHashTree(void);
extern void freeHashTree(void);

int hash(char *s);
hashtbl *findNode(hashtbl *link, char *s);
void insertNode(hashtbl **link, char *s, int t);
void deleteAllNodes(hashtbl **link);
hashtbl *queryHashNode(char *s);

/* function */
void initHashTree(void)
{  
   	DBGPRT("Function: initHashTree\r\n");
   	int i;
   	for(i=0;i<PRIME;i++)
   	{
      		hashtree[i].empty=true;
      		memset(hashtree[i].src,0,TOKENSIZE);
      		hashtree[i].type=NONETYPE; 
      		hashtree[i].value=TRUE;
		hashtree[i].str=NULL;
		hashtree[i].list=NULL;
      		hashtree[i].left=NULL;
      		hashtree[i].right=NULL;
   	}/* end for */
   	DBGPRT("End initHashTree\r\n");
   	return;
}/* end inithashTree */

void freeHashTree(void)
{  
   DBGPRT("Function: freeHashTree\r\n");
   int i;
   for(i=0;i<PRIME;i++)
   {
   	deleteAllNodes(&(hashtree[i].left));
        deleteAllNodes(&(hashtree[i].right));
   }/* end for */
   DBGPRT("End freeHashTree\r\n");
   return;
}/* end freeHashTree */

void addNodeToTree(char *s, int t)
{  
   	DBGPRT("Function: addNodeToTree\r\n");
   	DBGPRT1("addNodeToTree: the argument: s= %s\r\n",s);
   	DBGPRT1("addNodeToTree: the argument: t= %d\r\n",t);
   	int hash_value;
   	hash_value=hash(s);
   	DBGPRT1("addNodeToTree: after hash: hash_value= %d\r\n",hash_value);
   	if(hashtree[hash_value].empty==true)
   	{  
      		DBGPRT("addNodeToTree: if(hashtree[hash_value].empty==true)\r\n");
      		hashtree[hash_value].empty=false;  
      		hashtree[hash_value].type=t;
      		strcpy(hashtree[hash_value].src,s);
		hashtree[hash_value].list=NULL;
		hashtree[hash_value].str=NULL;
		if(t==COMPLEXTYPE)
		{
			hashtree[hash_value].list=A_S(analyze_string_len);
		}/* end if */
		else if(t==STRINGTYPE)
		{
			hashtree[hash_value].str=A_S(STRINGSIZE);
		}/* end else if */
      		hashtree[hash_value].left=NULL;
     		hashtree[hash_value].right=NULL;
      		DBGPRT1("addNodeToTree: s= %s\r\n",hashtree[hash_value].src);
      		return;
   	}/* end if */
   	DBGPRT("addNodeToTree: hashtree[hash_value].empty==false\r\n");
   	hashtbl *ptr;
   	ptr=&hashtree[hash_value];
   	insertNode(&ptr, s, t);
   	DBGPRT("End addNodeToTree\r\n");
   	return;
}/* end addNodeToTree */

void insertNode(hashtbl **link, char *s, int t)
{  
   	DBGPRT("Function: insertNode\r\n");
   	DBGPRT1("insertNode: the argument: s= %s\r\n",s);
   	DBGPRT1("insertNode: the argument: t= %d\r\n",t);
   	if((*link)==NULL)
   	{	
      		DBGPRT("insertNode: (*link)==NULL\r\n");
      		(*link)=(hashtbl *)malloc(sizeof(hashtbl));
      		(*(*link)).empty=false;
      		strcpy((*(*link)).src,s);
      		(*(*link)).type=t;
      		(*(*link)).value=TRUE;
		(*(*link)).list=NULL;
		(*(*link)).str=NULL;
		if(t==COMPLEXTYPE)
		{
			(*(*link)).list=A_S(analyze_string_len);			
		}/* end if */
		else if(t==STRINGTYPE)
		{
			(*(*link)).str=A_S(STRINGSIZE);
		}/* end else if */
      		(*(*link)).left=NULL;
      		(*(*link)).right=NULL;
      		DBGPRT("insertNode: (*(*link)),empty=false\r\n");
     		DBGPRT1("insertNode: (*(*link)).src= %s\r\n",(*(*link)).src);
      		DBGPRT1("insertNode: (*(*link)).type= %d\r\n",(*(*link)).type);
   	}/* end if */
   	else if(strcmp(s,(*(*link)).src)==0)
   	{   
      		DBGPRT("insertNode: else if(strcmp(s,(*(*link)).src)==0)\r\n");
       		/* error */
       		/* the same name of valiable*/
   	}/* end else if */
   	else if(strcmp(s,(*(*link)).src)<0)
   	{   
      		DBGPRT("insertNode: else if(strcmp(s,(*(*link)).src)<0)\r\n");      
      		insertNode( &((*(*link)).left), s, t);
   	}/* end else if */
   	else
   	{  
      		DBGPRT("insertNode: else if(strcmp(s,(*(*link)).src)>0)\r\n");
      		insertNode( &((*(*link)).right), s, t);
   	}/* end else */
   	return;
}/* end insertNode */

int hash(char *s)
{  
   	DBGPRT("Function: hash\r\n");
   	DBGPRT1("hash: the argument: s= %s\r\n",s);
   	int i=0;
   	int len_s=strlen(s);
   	char hash_buf[TOKENSIZE]={0};
   	strcpy(hash_buf,s);
   	unsigned int hash_value=0;
   	for(;i<len_s;i++)
   	{  
      		hash_value+=(int)hash_buf[i];
   	}/* end if */
   	DBGPRT1("hash: hash_value= %d\r\n",hash_value);
   	DBGPRT("End hash\r\n");
   	return hash_value % PRIME;
}/* end hash */

hashtbl *queryHashNode(char *s)
{  
   	DBGPRT("Function: queryHashNode\r\n");
   	DBGPRT1("queryHashNode: the argument: s= %s\r\n",s);
   	int hash_value;
   	hash_value=hash(s);
   	DBGPRT1("queryhashNode: after hash: hash_value= %d\r\n",hash_value);
   	if(hashtree[hash_value].empty==true){return NULL;}
   	DBGPRT("End queryHashNode\r\n");
   	return findNode(&hashtree[hash_value],s);
}/* end queryHashNode */

hashtbl *findNode(hashtbl *link, char *s)
{   
   	DBGPRT("Function: findNode\r\n");
   	if(link==NULL)
   	{
      		return NULL;
   	}/* end if */
   	else if(strcmp(s,(*link).src)==0)
   	{
      		return link;
   	}/* end else if */
   	else if(strcmp(s,(*link).src)>0)
   	{
      		return findNode((*link).right,s);
   	}/* end else if */
   	else
   	{
      		return findNode((*link).left,s);
   	}/* end else */
   	DBGPRT("End findNode\r\n");
}/* end findNode */

void deleteAllNodes(hashtbl **link)
{
	DBGPRT("Function: deleteAllNodes\r\n");
	if(*link==NULL){ return; }
	deleteAllNodes(&(*(*link)).left);
	deleteAllNodes(&(*(*link)).right);
	free(*link);
	*link=NULL;
	DBGPRT("End deleteAllNodes\r\n");
	return;
}/* end deleteAllNodes */

int getNodeType(char *s)
{
   	DBGPRT("Function: getNodeType\r\n");
   	DBGPRT1("getNodeType: the argument: s= %s\r\n",s);
	hashtbl *ptr;
	ptr=queryHashNode(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
   	DBGPRT("End getNodeType\r\n");	
	return (*ptr).type;
}/* end getNodeType*/

void putStringToNode(char *s, char *str)
{
   	DBGPRT("Function: getNodeType\r\n");
   	DBGPRT1("getNodeType: the argument: s= %s\r\n",s);	
   	DBGPRT1("getNodeType: the argument: str= %s\r\n",str);
	hashtbl *ptr;
	ptr=queryHashNode(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
	strcpy((*ptr).str,str);
   	DBGPRT("End getNodeType\r\n");
	return;
}/* end putStringToNode */

void putIntValToNode(char *s, int v)
{
   	DBGPRT("Function: putIntValToNode\r\n");
   	DBGPRT1("putIntValToNode: the argument: s= %s\r\n",s);	
   	DBGPRT1("putIntValToNode: the argument: v= %d\r\n",v);
	hashtbl *ptr;
	ptr=queryHashNode(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
	(*ptr).value=v;
   	DBGPRT("End putIntValToNode\r\n");	
	return;	
}/* end putIntValToNode */

char *getStringFromNode(char *s)
{
   	DBGPRT("Function: getStringFromNode\r\n");
   	DBGPRT1("getStringFromNode: the argument: s= %s\r\n",s);	
	hashtbl *ptr;
	ptr=queryHashNode(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
   	DBGPRT("End getStringFromNode\r\n");	
	return (*ptr).str;
}/* end getStringFromNode */

int getIntValFromNode(char *s)
{
   	DBGPRT("Function: getIntValFromNode\r\n");
   	DBGPRT1("getIntValFromNode: the argument: s= %s\r\n",s);
	hashtbl *ptr;
	ptr=queryHashNode(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
   	DBGPRT("End getIntValFromNode\r\n");
	return (*ptr).value;
}/* end getIntValFromNode */

void putExpressionToNode(char *s, char *e)
{
   	DBGPRT("Function: putExpressionToNode\r\n");
   	DBGPRT1("putExpressionToNode: the argument: s= %s\r\n",s);
   	DBGPRT1("putExpressionToNode: the argument: t= %s\r\n",t);
	hashtbl *ptr;
	int pos;
	char *local;
	ptr=queryHashNode(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
	if((*ptr).type==COMPLEXTYPE)
	{
		local=strchr(e,'=');
		pos=(local-e)+1;
		strcpy((*ptr).list,e+pos);
	}/* end if */
   	DBGPRT("End putExpressionToNode\r\n");	
	return;
}/* end putExpressionToNode */

char *getExpressionStringFromNode(char *s)
{
	hashtbl *ptr;
	ptr=queryHashNode(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
	if((*ptr).type!=COMPLEXTYPE)
	{
		/* error */
	}/* end if */
	return (*ptr).list;
}/* end getExpressionStringFromNode */







