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
