/******************************************  
	File name	: zynfunc.c
	Description : personal function library
	Author		: zhaoyining
	Data		: 2019-08-03
	History		:
******************************************/
#include "./zynfunc.h"
#include <stdio.h>
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
	if( n < 3 || 0 == (n & 1) )
		return 0;
	int i = 0;
	for( i = 3; i <= n >> 1; i += 2 )
		if( 0 == n % i )
			return 0;
	return 1;
}
