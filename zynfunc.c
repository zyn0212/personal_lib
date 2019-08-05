/******************************************  
	File name	: zynfunc.c
	Description : personal function library
	Author		: zhaoyining
	Data		: 2019-08-03
	History		:
******************************************/
#include "./zynfunc.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
static int check_xornode(HEADER *header, NODE_SMP *node);
extern int zyn_test(void)
{
	printf("test OK: %d!\n", zyn_gcd(134, 26));
	return 0;
}
extern int zyn_gcd(int a, int b)
{
	if( 0 == a || 0 == b )
		return 0;
	if( 0 == a % b )
		return b;
	if( 0 == b % a )
		return a;
	int tmp = 0;
	while( 0 != (tmp = a % b) ) {
		a = b;
		b = tmp;
	}
	return b;
}
extern int zyn_lcm(int a, int b)
{
	if( 0 == a || 0 == b )
		return 0;
	int t = zyn_gcd(a, b);
	if( 1 == zyn_isOverFlow_mul(a, b / t) )
		return 0;

	return a * (b / t);
}
extern int zyn_isPrime(int n)
{
	if( n < 2 )
		return 0;
	if( 2 == n || 3 == n )
		return 1;
	if( 1 != n % 6 && 5 != n % 6 )
		return 0;
	int i = 5;
	for( ; i < sqrt(n); i += 6 )
		if( 0 == n % i || 0 == n % (i + 2) )
			return 0;
	return 1;
}
extern long zyn_pow(int base, int x)
{
	if ( 0 == base )
		return 0;
	if( 0 == x )
		return 1;
	long ret = 1, tmp_base = (long)base;
	for( ; x > 0; tmp_base *= tmp_base, x >>= 1 )
		if( 1 == zyn_isOverFlow_mul(ret, tmp_base) || 1 == zyn_isOverFlow_mul(tmp_base, tmp_base) ) {
			return 0; }
		else if( 1 == (x & 1) )
			ret *= tmp_base;
	return ret;
}
int zyn_insertSort(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), SORT_METHOD sortby)
{
	if( NULL == unsort || n < 2 || size < 1 || NULL == compare )
		return -1;
	void* sorted = NULL;
	void* key = NULL;
	if( NULL == (sorted = calloc(n, size)) || NULL == (key = calloc(1, size)) )
		return -2;
	int i = 0, j = 0, moved = 0;
	memcpy(sorted, unsort, size);
	for( i = 1; i < n; i += 1 )
	{
		memcpy(key, unsort + i * size, size);
		j = i;
		while( j > 0 && (compare( key, sorted + (j - 1) * size) * sortby) > 0 )
		{
			j -= 1;
		}
		moved += i != j;
		memmove(sorted + (j + 1) * size, sorted + j * size, (i - j) * size);
		memcpy(sorted + j * size, key, size);
	}
	memcpy(unsort, sorted, n * size);
	free(sorted);
	free(key);
	return moved;
}
int zyn_bubbleSort(void *unsort, int n, int size, int (*compare)(void const *, void const *), SORT_METHOD sortby)
{
	if( n < 2 || size < 1 || NULL == unsort )
		return -1;
	void *temp = NULL;
	void *sorted = NULL;
	if( NULL == (temp = calloc(1, size)) || NULL == (sorted = calloc(n, size)) )
		return -2;
	memcpy(sorted, unsort, n * size);
	int i = 0, j = 0, change = 0, moved = 0;
	for( i = 1; i < n; i += 1, change = 0)
	{
		j = n - 1;
		while( j >= i )
		{
			if( compare(sorted + j * size, sorted + (j - 1) * size) * sortby > 0 )
			{
				memcpy(temp, sorted + j * size, size);
				memcpy(sorted + j * size, sorted + (j - 1) * size, size);
				memcpy(sorted + (j - 1) * size, temp, size);
				moved += 1;
				change = 1;
			}
			j -= 1;
		}
		if( 0 == change )
			break;
	}
	memcpy(unsort, sorted, n * size);
	free(temp);
	free(sorted);
	return moved;
}
extern HEADER * zyn_initxorll(NODE_SMP * head, NODE_SMP * tail)
{
	if( NULL == head || NULL == tail )
		return NULL;
	HEADER header_tmp = {2, head, tail};
	HEADER *header_ret = (HEADER *)calloc(1, sizeof(HEADER));
	if( NULL == header_ret ) {
		return NULL;
	}
	return (HEADER *)memcpy(header_ret, &header_tmp, sizeof(HEADER));
}
extern int zyn_destoryxorll(HEADER * header)
{
	int num = 0;
	if( NULL == header )
		return num;
	NODE_SMP * prev = header->head, * curr = XOR_2PTR(prev->side, header->tail);
	NODE_SMP * next = XOR_2PTR(curr->side, prev);
	while( header->tail != curr ) {
		prev = curr;
		curr = next;
		next = XOR_2PTR(curr->side, prev);
		free(prev);
		++num;
	}
	free(header->head);
	free(header->tail);
	num += 2;
	free(header);
	return -num;
}
extern NODE_SMP * zyn_insertxornode(HEADER * header, NODE_SMP * prev, NODE_SMP * next, NODE_SMP * insnode)
{
	if( NULL == header || NULL == prev || NULL == next || NULL == insnode )
		return NULL;
	int prevloc = check_xornode(header, prev);
	int nextloc = check_xornode(header, next);
	if( -1 != prevloc && -1 != nextloc && (1 == prevloc - nextloc || 1 == nextloc - prevloc) ) {
		prev->side = XOR_3PTR(prev->side, insnode, next);
		insnode->side = XOR_2PTR(prev, next);
		next->side = XOR_3PTR(next->side, prev, insnode);
		++header->nodenum;
		return insnode;
	}
	return NULL;
}
extern NODE_SMP * zyn_deletexornode(HEADER * header, NODE_SMP * prev, NODE_SMP * next, NODE_SMP * delnode)
{
	if( NULL == header || NULL == prev || NULL == next || NULL == delnode )
		return NULL;
	if( header->head == delnode || header->tail == delnode )
		return NULL;
	int prevloc = check_xornode(header, prev);
	int nextloc = check_xornode(header, next);
	int delloc = check_xornode(header, delnode);
	if( -1 != prevloc && -1 != nextloc && -1 != delloc && delloc << 1 == prevloc + nextloc ) {
		prev->side = XOR_3PTR(prev->side, delnode, next);
		delnode->side = XOR_2PTR(prev, next);
		next->side = XOR_3PTR(next->side, prev, delnode);
		--header->nodenum;
		return delnode;
	}
	return NULL;
}
extern int zyn_printxorll(HEADER *header, void (*prtnode)(NODE_SMP *node, int nodeloc), PRTLL_METHOD prtmethod)
{
	if( NULL == header )
		return -1;
	NODE_SMP * prev = NULL, * next = NULL, * current = NULL, * end = NULL;
	if( PRTLL_H2T == prtmethod ) {
		prev = header->tail;
		current = header->head;
		end = header->tail;
	}
	else if( PRTLL_T2H == prtmethod ) {
		prev = header->head;
		current = header->tail;
		end = header->head;
	}
	int loc = 0;
	do {
		if( NULL == prtnode )
			printf("%d\t%d\n", loc, current->val);
		else
			prtnode(current, loc);
		next = XOR_2PTR(current->side, prev);
		prev = current;
		current = next;
		next = NULL;
		++loc;
	} while( end != current );
	if( NULL == prtnode )
		printf("%d\t%d\n", loc, current->val);
	else
		prtnode(current, loc);
	return loc + 1;
}
static int check_xornode(HEADER *header, NODE_SMP *node)
{
	if( header->head == node )
		return 0;
	NODE_SMP * prev = header->tail, * current = header->head, * next = NULL;
	int loc = 0;
	do {
		++loc;
		next = XOR_2PTR(current->side, prev);
		prev = current;
		current = next;
		next = NULL;
		if( current == node )
			return loc;
	} while( header->tail != current );
	return -1;
}
