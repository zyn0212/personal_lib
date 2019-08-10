#include <limits.h>
#ifndef _LIBZYNFUNC_H
#define _LIBZYNFUNC_H
typedef enum {
	SORT_LTOS = 1,
	SORT_STOL = -1
} SORT_METHOD;
typedef enum {
	PRTLL_H2T,
	PRTLL_T2H
} PRTLL_METHOD;
typedef struct node_smp {
	int val;
	struct node_smp * side;
} NODE_SMP;
typedef struct {
	int nodenum;
	NODE_SMP * const head;
	NODE_SMP * const tail;
} HEADER;
#define XOR_2PTR(a, b) ((NODE_SMP*)((long)(a)^(long)(b)))
#define XOR_3PTR(a, b, c) ((NODE_SMP*)((long)(a)^(long)(b)^(long)(c)))

#ifdef __cplusplus
 extern "C" {
#endif
#define zyn_isOverFlow_mul(x, y) ((x)*(y)/(x) != (y))
#define zyn_isOverFlow_add(x, y) ((((x)^(y))|(((~((x)^(y))&1<<sizeof(long)*CHAR_BIT-1)^(x))+(y))^(y))>=0) 
#define HUGE_NUMBER_LENGTH 10000
extern int zyn_test(void);
extern int zyn_gcd(int a, int b);
extern int zyn_lcm(int a, int b);
extern int zyn_isPrime(int n);
extern long zyn_pow(int base, int x);
extern int zyn_insertSort(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), SORT_METHOD sortby);
extern int zyn_bubbleSort(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), SORT_METHOD sortby);
extern HEADER * zyn_initxorll(NODE_SMP * head, NODE_SMP * tail);
extern int zyn_destoryxorll(HEADER * header);
extern int zyn_printxorll(HEADER *header, void (*prtnode)(NODE_SMP *node, int nodeloc), PRTLL_METHOD prtmethod);
extern NODE_SMP * zyn_insertxornode(HEADER * header, NODE_SMP * prev, NODE_SMP * next, NODE_SMP * insnode);
extern NODE_SMP * zyn_deletexornode(HEADER * header, NODE_SMP * prev, NODE_SMP * next, NODE_SMP * delnode);
extern char * zyn_huge_plus(char const * a, char const * b, char * result, int maxlen);
extern char * zyn_huge_minus(char const * a, char const * b, char * result, int maxlen);
extern char * zyn_huge_time(char * const a, char * const b, char * result, int maxlen);

#ifdef __cplusplus
 }
#endif

#endif
