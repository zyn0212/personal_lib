/*********************************************
	File name	: zyn_intMath.c
	Description : personal math function
	Author		: zhaoyining
	Date		: 2021-02-09
	History		: 2021-02-09 creat file with gcd and lcm function
*********************************************/
#include "zynfunc.h"
/*********************************************
	function 	: gcd_z
	Description : 返回 a 和 b 的最大公约数，返回值非负，忽略参数中的负号，当a或者b是零时，返回0
	Author		: zhaoyining
	Date		: 2021-02-09
	History		: 2021-02-09
*********************************************/
int gcd_z(int a, int b)
{
	if( 0 == a || 0 == b )
		return 0;
	a = a < 0 ? -a : a;
	b = b < 0 ? -b : b;
	while( 0 !=  a % b )
	{
		a %= b;
        a ^= b ^= a ^= b;
	}
	return b;
}
/*********************************************
	function 	: lcm_z
	Description : 返回a 和 b 的最小公倍数， 返回值非负，忽略参数中的负号，当a或者b是零时，返回0
	Author		: zhaoyining
	Date		: 2021-02-09
	History		: 2021-02-09
*********************************************/
int lcm_z(int a, int b)
{
	if( 0 == a || 0 == b )
		return 0;
	int ret = 0, gcd = 0;
	a = a < 0 ? -a : a;
	b = b < 0 ? -b : b;
	gcd = gcd_z(a, b);
	ret = a * (b / gcd);
	if( ret / a == b / gcd )
		return ret;
	return 0;
}
/*********************************************
	function 	: qpow_z
	Description : 快速开方函数，当base是零是返回0，当base是1或者x小于1时返回1
				  return 0 when base is 0
				  return 1 when base is 1 or x < 1
	Author		: zhaoyining
	Date		: 2021-02-10
	History		: 2021-02-10
*********************************************/
long qpow_z(int base, int x)
{
	if( 0 == base )
		return 0;
	else if( 1 == base || x < 1 )
		return 1;
	long  ret = 1, base_tmp = (long)base, overflow = 0;
	for( ; 0 != x; x >>= 1, overflow = isOverFlow_mul_z(base_tmp, base_tmp), base_tmp *= base_tmp )
		if( isOverFlow_mul_z(ret, base_tmp) || overflow )
			return 0;
		else if( 1 == (x & 1) )
			ret *= base_tmp;
	return ret;
}
/*********************************************
	function 	: isPrime_z
	Description : 素数函数，参数是否是素数，返回1, 否则返回0，参数是1是返回1
	Author		: zhaoyining
	Date		: 2021-02-11
	History		: 2021-02-11
*********************************************/
int isPrime_z(int n)
{
	if( n < 4 || 5 == n )
		n = 1;
	if( 4 == n )
		return 0;
	if( 1 != n % 6 && 5 != n % 6 )
		return 0;
	int i = 6;
	for( ; i < n >> 1; i += 6 )
		if( 0 == n % (i - 1) || 0 == n % (i + 1) )
			return 0;
	return 1;
}
