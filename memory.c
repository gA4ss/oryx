/* memory.c */
#include "headfile.h"
#include "debug.h"
#include "config.h"
#include "alloc.h"
#include "consequence_token.h"

#define MEMORY_PRIME		5

typedef struct page_types
{	
	bool empty;
	int  *token_ram;	
	int  id;
	int  type;
	struct page_types *left;
	struct page_types *right;
}page;

page memory_tree[MEMORY_PRIME];

extern int token_ram_len;			/* run.c */
extern int analyze_string_len;			/* run.c */
extern int symbol_atom_number;			/* run.c */
extern char **object_symbol;			/* run.c */

/* Function */
extern void addPageToMemory(char *s, int id, int type, int *ram);
extern int  getTypeFromId(int id);
extern int  getIdFromSrc(char *s);
extern int  *getTokenRamFromId(int id);
extern int  getTypeFromSrc(char *s);
extern bool makeSureSrcBeRecorded(char *s);

extern void initMemoryTree(void);
extern void freeMemoryTree(void);

int  hashPage(char *s);
void insertPage(page **link, char *s, int id, int type, int *ram);
void deleteAllPages(page **link);
page *findPage(page *link, char *s);
page *queryHashPage(char *s);

void initMemoryTree(void)
{
	int i;
	for(i=0;i<MEMORY_PRIME;i++)
	{	
		memory_tree[i].empty=true;
		memory_tree[i].token_ram=NULL;		
		memory_tree[i].id=-1;
		memory_tree[i].type=NONEOBJECT;
		memory_tree[i].left=NULL;
		memory_tree[i].right=NULL;
	}/* end for */
	for(i=0;i<symbol_atom_number;i++)
	{		
		memset(object_symbol[i], 0, sizeof(char)*analyze_string_len);
	}/* end for */
	return;
}/* end initMemoryTree */

void freeMemoryTree(void)
{
   	int i;
   	for(i=0;i<MEMORY_PRIME;i++)
   	{	
		memory_tree[i].empty=true;
		if(memory_tree[i].token_ram!=NULL)free(memory_tree[i].token_ram);
		memory_tree[i].token_ram=NULL;
		memory_tree[i].type=NONEOBJECT;
		memory_tree[i].id=-1;
   		deleteAllPages(&(memory_tree[i].left));
        	deleteAllPages(&(memory_tree[i].right));
   	}/* end for */	
	/* free object_symbol */
   	return;	
}/* end freeMemoryTree */

void addPageToMemory(char *s, int id, int type, int *ram)
{	
	int hash_value=hashPage(s);
	if(memory_tree[hash_value].empty==true)
	{
		memory_tree[hash_value].empty=false;
		memory_tree[hash_value].type=type;
		memory_tree[hash_value].id=id;
		memory_tree[hash_value].left=NULL;
		memory_tree[hash_value].right=NULL;
		if(type!=ATOM)
		{
			memory_tree[hash_value].token_ram=A_I_L(token_ram_len);
			memcpy(memory_tree[hash_value].token_ram,ram, (sizeof(int)*token_ram_len));
		}/* end if */
		return;
	}/* end if */
	page *ptr;
	ptr=&memory_tree[hash_value];
	insertPage(&ptr,s,id,type,ram);
	return;
}/* end addPageToMemory */

void insertPage(page **link, char *s, int id, int type, int *ram)
{
	if((*link)==NULL)
	{
		(*link)=(page *)(malloc(sizeof(page)));
		(*(*link)).empty=false;
		(*(*link)).type=type;
		(*(*link)).id=id;
		(*(*link)).left=NULL;
		(*(*link)).right=NULL;
		if(type!=ATOM)
		{	
			(*(*link)).token_ram=A_I_L(token_ram_len);
			memcpy((*(*link)).token_ram, ram, (sizeof(int)*token_ram_len));
		}/* end if */
	}/* end if */
	else if(strcmp(s, object_symbol[(*(*link)).id])<0)
	{
		insertPage(&((*(*link)).left), s, id, type, ram);
	}/* end else if */
	else
	{
		insertPage(&((*(*link)).right), s, id, type, ram);		
	}/* end else if */
	return;
}/* end insertPage */

int  hashPage(char *s)
{  
   	int i=0;
   	int len_s=strlen(s);
   	char *hash_buf;
	hash_buf=A_S(analyze_string_len);
	memset(hash_buf, 0, analyze_string_len);
   	strcpy(hash_buf, s);
   	unsigned int hash_value=0;
   	for(;i<len_s;i++)
   	{  
      		hash_value+=(int)hash_buf[i];
   	}/* end if */
	free(hash_buf);
   	return hash_value % MEMORY_PRIME;
}/* end hashPage */

page *queryHashPage(char *s)
{  
   	int hash_value=hashPage(s);
   	if(memory_tree[hash_value].empty==true){return NULL;}
   	return findPage(&memory_tree[hash_value], s);
}/* end queryHashPage */

page *findPage(page *link, char *s)
{   
   	if(link==NULL)
   	{
      		return NULL;
   	}/* end if */
   	else if(strcmp(s, object_symbol[(*link).id])==0)
   	{
      		return link;
   	}/* end else if */
   	else if(strcmp(s, object_symbol[(*link).id])<0)
   	{
      		return findPage((*link).left, s);
   	}/* end else if */
   	else
   	{
      		return findPage((*link).right, s);
   	}/* end else */
   	DBGPRT("End findPage\r\n");
}/* end findPage */

void deleteAllPages(page **link)
{
	if(*link==NULL){ return; }
	if((*(*link)).token_ram!=NULL)free((*(*link)).token_ram);
	deleteAllPages(&(*(*link)).left);
	deleteAllPages(&(*(*link)).right);
	free(*link);
	*link=NULL;
	return;
}/* end deleteAllPages */

int getTypeFromId(int id)
{
	page *ptr;
	ptr=queryHashPage(object_symbol[id]);
	if(ptr==NULL)
	{
		return NONEOBJECT;
	}/* end if */
	return (*ptr).type;
}/* end getTypeFromId */

int getIdFromSrc(char *s)
{
	page *ptr;
	ptr=queryHashPage(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
	return (*ptr).id;
}/* end getIdFromSrc */

int *getTokenRamFromId(int id)
{
	page *ptr;
	ptr=queryHashPage(object_symbol[id]);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
	return (*ptr).token_ram;
}/* end getTokenRamFromId */

int  getTypeFromSrc(char *s)
{
	page *ptr;
	ptr=queryHashPage(s);
	if(ptr==NULL)
	{
		/* error */
	}/* end if */
	return (*ptr).type;
}/* end getTypeFromSrc */

bool makeSureSrcBeRecorded(char *s)
{
	page *ptr;
	ptr=queryHashPage(s);
	if(ptr!=NULL){return true;}
	return false;
}/* end makeSureSrcBeRecorded */



