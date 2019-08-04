#include <limits.h>
#ifndef _LIBZYNFUNC_H
#define _LIBZYNFUNC_H
#ifdef __cplusplus
 extern "C" {
#endif
#define zyn_isOverFlow_mul(x, y) ((x)*(y)/(x) != (y))
#define zyn_isOverFlow_add(x, y) ((((x)^(y))|(((~((x)^(y))&1<<sizeof(long)*CHAR_BIT-1)^(x))+(y))^(y))>=0) 
extern int zyn_test(void);
extern int zyn_gcd(int a, int b);
extern int zyn_lcm(int a, int b);
extern int zyn_isPrime(int n);
extern long zyn_pow(int base, int x);

#ifdef __cplusplus
 }
#endif

#endif
