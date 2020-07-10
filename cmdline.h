/* cmdline.h */
typedef struct cmdline_types
{
 	char filename[MAX_PATH];		/* filename for execute */
 
  	int token_ram_len;			/* the size of expression run ram */	
  	int compute_ram_len;			/* the size of expression string */
	int symbol_atom_number;			/* the size of atom */
	int analyze_string_len;			/* the size of analyze_token_ram */
	int equfigure_list_len;			/* the equfigure list number */
}cmdline;
