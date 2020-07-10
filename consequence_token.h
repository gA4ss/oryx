/* consequence_token.h */

#define NOT             4000      	/* ~ */
#define AND             4002     	/* && */
#define OR              4003       	/* || */
#define XOR             4004       	/* ^ */
#define EQV             4005       	/* <-> */
#define IMP             4006       	/* -> */
#define PREAND          4007       	/* |> */
#define EQP		4028		/* <=> */

#define LPAREM          4008       	/* ( */
#define RPAREM          4009       	/* ) */
#define BLPAREM         4010      	/* { */
#define BRPAREM         4011      	/* } */
#define SEMICOLON       4012      	/* ; */
#define ASSIGNOP        4013      	/* = */
#define COMMA           4014      	/* , */

#define ID		5000

#define IDA		5001			
#define IDB		5002
#define IDC		5003

#define NONEOBJECT	-1	/* opt and others */

#define ATOM		-20	/* p */
#define NOT_ATOM	-21	/* ~p */
#define PAREM		-22	/* (P||q) */
#define NOT_PAREM	-23	/* ~(p||q) */
#define OBJECT		-24	/* XXX opt YYY */
#define PROJECT		-25
#define TOKENRAM_END	-26


