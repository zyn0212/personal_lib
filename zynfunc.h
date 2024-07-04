#include <limits.h>
#ifndef _LIBZYNFUNC_H
#define _LIBZYNFUNC_H
typedef struct node_smp_ {
    void* data;
	int size;
	struct node_smp_* side;
} NODE_SMP;
typedef struct {
	int nodeNum;
	NODE_SMP * const first;
	NODE_SMP * const second;
} HEADER;

#ifdef __cplusplus
 extern "C" {
#endif
#define zyn_isOverFlow_mul(x, y) ((x)*(y)/(x) != (y))
#define zyn_isOverFlow_add(x, y) ((((x)^(y))|(((~((x)^(y))&1<<(sizeof(long)<<3)-1)^(x))+(y))^(y))>=0) 
#define isOverFlow_mul_z(x, y) ((x)*(y)/(x) != (y))
#define isOverFlow_add_z(x, y) ((((x)^(y))|(((~((x)^(y))&1<<(sizeof(long)<<3)-1)^(x))+(y))^(y))>=0) 

// XOR linked list function
extern HEADER* initXorLinkedList(NODE_SMP* head, NODE_SMP* next, int nodeNum);
extern int destoryXorLLHeader(HEADER * header);
extern NODE_SMP* insertNewXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* newNode, NODE_SMP* next);
extern NODE_SMP* addNewXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* current, NODE_SMP* newNode);
extern NODE_SMP* addXorNodeBetweenF_S(HEADER* header, NODE_SMP* prev, NODE_SMP* newNode);
extern NODE_SMP* deleteXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* deleteNode, NODE_SMP* next);
extern NODE_SMP* getNextXorNode(NODE_SMP* prev, NODE_SMP* current);

//math
extern int gcd_z(int a, int b);
extern int lcm_z(int a, int b);
extern long qpow_z(int base, int x);
extern int isPrime_z(int n);

// sort function
extern int BubbleSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int SelectSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int InsertSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int ShellSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int MergeSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int QuickSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int CountSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int BucketSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int RadixSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);
extern int HeapSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall);

// huge number math base on string
#define STRING_NUMBER_MAX_LEN 200
#define STRING_NUMBER_MAX_NUM 300
extern char* strPlus(char* a, char* b, int base, char* result);
extern char* strMinus(char* a, char* b, int base, char* result);
extern char* strTime(char* a, char* b, int base, char* result);
extern char* strDivide(char* a, char* b, int base, int precision, char* result);
// interest function
/*
   0       1
    1  2  3
    8  9  4
    7  6  5
   3       2
*/
extern int fillTrangl(int startnum, int tranglWide, int* array, int arrayWide, int corner);
extern int convertHex2Oct(char* hex, char* result);
extern int zyn_cvtDecToX(unsigned long long s, unsigned int base, char* result, int maxLen);
#ifdef __cplusplus
 }
#endif

#endif
