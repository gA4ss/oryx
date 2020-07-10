/* token.h */
#define NOT             4000       /* ~ */
#define AND             4002     /* && */
#define OR              4003       /* || */
#define XOR             4004       /* ^ */
#define EQV             4005       /* <-> */
#define IMP             4006       /* -> */
#define PREAND          4007       /* |> */

#define LPAREM          4008       /* ( */
#define RPAREM          4009       /* ) */
#define BLPAREM         4010      /* { */
#define BRPAREM         4011      /* } */
#define SEMICOLON       4012      /* ; */
#define ASSIGNOP        4013      /* = */
#define COMMA           4014      /* , */
#define KEEP1           4015
/* HIDE TOKEN */                /* JUST USE IN ADD NODE TO HASH TREE */
#define ATOMTYPE        4016      /* ATOM */
#define COMPLEXTYPE     4017      /* COMPLEX */
#define PREDICATETYPE   4018      /* PRED */
#define STRINGTYPE      4019      /* STRING */
/* #define TABLETYPE	4020	*/
#define PROCEDURETYPE   4021     /* PROCEDURE */

#define LOGICLABEL      4022      /* ? */
/* CONST DEFINE */
#define TRUEVAL         4023
#define FALSEVAL        4024
#define STRINGVAL       4025
/* KEY TOKEN */
#define X		4026
/* CONTROL TOKEN */
#define NEW_PROCEDURE	4027
/* EX TOKEN */
#define EQP		4028
/* END TOKEN */
#define SCAN_ERROR      4029
#define SCANEOF         4030
/* End Token Macro */
