/* figure_memory.c */
#include "headfile.h"
#include "debug.h"
#include "config.h"
#include "alloc.h"
#include "consequence_token.h"

#define FIGURE_MEMORY_PRIME	5

typedef struct figurepage_types
{
	bool empty;
	int  id;
	int  number;
	
	struct figurepage_types *left;
	struct figurepage_types *right;
}figurepage;

figurepage figurememory_tree[FIGURE_MEMORY_PRIME];

extern int token_ram_len;			/* run.c */
extern int analyze_string_len;			/* run.c */
extern char **equ_figure_symbol;		/* run.c */

