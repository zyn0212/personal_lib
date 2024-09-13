#include <limits.h>
#ifndef _LIBZYNFUNC_H
#define _LIBZYNFUNC_H
#define STRING_NUMBER_MAX_LEN 200

#ifdef __cplusplus
 extern "C" {
#endif
#define zyn_isOverFlow_mul(x, y) ((x)*(y)/(x) != (y))
#define zyn_isOverFlow_add(x, y) ((((x)^(y))|(((~((x)^(y))&1<<(sizeof(long)<<3)-1)^(x))+(y))^(y))>=0) 
#define isOverFlow_mul_z(x, y) ((x)*(y)/(x) != (y))
#define isOverFlow_add_z(x, y) ((((x)^(y))|(((~((x)^(y))&1<<(sizeof(long)<<3)-1)^(x))+(y))^(y))>=0) 
typedef struct _node_smp {
    void* data;
	int size;
	struct _node_smp* side;
} XOR_NODE;
typedef struct {
	int nodeNum;
	XOR_NODE* first;
	XOR_NODE* second;
} XOR_HEADER;

// XOR linked list function
extern XOR_HEADER* initXorLinkedList(XOR_NODE* first, XOR_NODE* second);
extern int destoryXorLLHeader(XOR_HEADER * header);
extern XOR_NODE* insertXorNode(XOR_HEADER* header, XOR_NODE* prev, XOR_NODE* newNode, XOR_NODE* next, XOR_NODE* confirm);
extern XOR_NODE* deleteXorNode(XOR_HEADER* header, XOR_NODE* prev, XOR_NODE* node, XOR_NODE* next);
extern XOR_NODE* getNextXorNode(XOR_NODE* prev, XOR_NODE* current);

//math
extern int zyn_gcd(int a, int b);
extern int zyn_lcm(int a, int b);
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
extern char* strPlus(char* a, char* b, int base, char* result);
extern char* strMinus(char* a, char* b, int base, char* result);
extern char* strTime(char* a, char* b, int base, char* result);
extern char* strDivide(char* a, char* b, int base, char* quot, char* rem);
extern char* strGcd(char* a, char* b, int base, char* result);
extern char* strLcm(char* a, char* b, int base, char* result);
extern char* strPow(char* a, int base, int x, char* result);
extern char* strBaseCvt(const char* a, int srcBase, int dstBase, char* result);
extern char* zyn_kmp(const char* s, const int slen, const char* p, const int plen);
// interest function
/*
   0       1
    1  2  3
    8  9  4
    7  6  5
   3       2
*/
extern int fillTrangl(int startnum, int tranglWide, int* array, int arrayWide, int corner);
extern int magicSquare(int* arr, int arrWide, int sqWide);
#ifdef __cplusplus
 }
#endif

#endif
