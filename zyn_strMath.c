/*********************************************
	File name	: zyn_strMath.c
	Description : personal huge number calculate function base on string
	Author		: zhaoyining
	Date		: 2022-07-05
	History		: 2022-07-05 creat file
	History		: 2024-08-24 add string KMP alog 
*********************************************/
#include "zynfunc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PRECISION_MAX 200
typedef struct {
    int compResult;
    int aSign;
    int aPrefix;
    int aValidLen;
    int bSign;
    int bPrefix;
    int bValidLen;
} STR_ABS_COMP_STATUS;
static int _getStrAbsStatus(char* a, char* b, int base, STR_ABS_COMP_STATUS* result);
static int _strNumberCheck(char c, int base);
static char* _strPlus(char* a, char* b, int base, char* result);
static char* _strMinus(char* a, char* b, int base, char* result);
static int _getNext(const char* s, const int len, int* next);
char* strPlus(char* a, char* b, int base, char* result)
{
    char* ret = result;
    STR_ABS_COMP_STATUS absResult = {0};
    if( 0 != _getStrAbsStatus(a, b, base, &absResult) ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    char* max = 1 == absResult.compResult ? a : b, * min = 1 == absResult.compResult ? b : a;
    if( -1 == absResult.compResult && -1 == absResult.aSign || 1 == absResult.compResult && -1 == absResult.bSign )
        *ret++ = '-';
    absResult.aSign == absResult.bSign ? _strPlus(max, min, base, ret) : _strMinus(max, min, base, ret);
    return result;
}
char* strMinus(char* a, char* b, int base, char* result)
{
    char* ret = result;
    STR_ABS_COMP_STATUS absResult = {0, 0, 0, 0, 0};
    if( 0 != _getStrAbsStatus(a, b, base, &absResult) ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    char* max = 1 == absResult.compResult ? a : b, * min = 1 == absResult.compResult ? b : a;
    if( 1 == absResult.compResult && -1 == absResult.aSign || -1 == absResult.compResult && 1 == absResult.bSign || 0 == absResult.compResult && -1 == absResult.aSign && 1 == absResult.bSign )
        *ret++ = '-';
    absResult.aSign != absResult.bSign ? _strPlus(max, min, base, ret) : _strMinus(max, min, base, ret);
    return result;
}
char* strTime(char* a, char* b, int base, char* result)
{
    char* retp = result;
    STR_ABS_COMP_STATUS absResult = {0, 0, 0, 0, 0};
    if( 0 != _getStrAbsStatus(a, b, base, &absResult) ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    int isNegative = 0;
    if( absResult.aSign != absResult.bSign )
        *retp++ = '-', isNegative = 1;
    if( absResult.aValidLen + absResult.bValidLen + 2 + isNegative >= STRING_NUMBER_MAX_LEN - 1 )
        return NULL;
    int aLen = 0, bLen = 0, resLen = 0, tmpResult[STRING_NUMBER_MAX_LEN], upbit = 0;
    memset(tmpResult, 0, sizeof tmpResult);
    for( ; aLen < absResult.aValidLen; resLen = resLen < aLen + bLen ? aLen + bLen : resLen, ++aLen )
        for( bLen = 0; bLen < absResult.bValidLen; ++bLen ) {
            int tmpa = 16 == base && 'a' <= (a[aLen] | 0x20) && (a[aLen] | 0x20) <= 'f' ? (a[aLen] | 0x20) - 'a' + 10 : a[aLen] - '0';
            int tmpb = 16 == base && 'a' <= (b[bLen] | 0x20) && (b[bLen] | 0x20) <= 'f' ? (b[bLen] | 0x20) - 'a' + 10 : b[bLen] - '0';
            tmpResult[aLen + bLen + 1] += tmpa * tmpb;
        }
    char tmpResultStr[STRING_NUMBER_MAX_LEN];
    div_t tmp = {0, 0};
    for( tmpResultStr[resLen + 1] = '\0'; resLen >= 0; --resLen ) {
        tmp = div(tmpResult[resLen] + tmp.quot, base);
        tmpResultStr[resLen] = 16 == base && tmp.rem > 9 ? tmp.rem - 10 + 'A' : tmp.rem + '0';
    }
    char* tmpRSp = tmpResultStr;
    while( '0' == *tmpRSp )
        ++tmpRSp;
    sprintf(retp, "%s", tmpRSp);
    return result;
}
char* strDivide(char* a, char* b, int base, int precision, char* result)
{
    char* resultp = result;
    STR_ABS_COMP_STATUS absResult = {0, 0, 0, 0, 0};
    if( 0 != _getStrAbsStatus(a, b, base, &absResult) || precision > PRECISION_MAX ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    char* checkZero = b;
    while( '0' == *checkZero )
        ++checkZero;
    if( '\0' == *checkZero ) {
        printf("divisor string is illegal!\n");
        return NULL;
    }
    if( absResult.aSign != absResult.bSign )
        *resultp++ = '-';
    char tmpUpNumber[STRING_NUMBER_MAX_LEN], tmpResult[STRING_NUMBER_MAX_LEN];
    char* retp = tmpResult;
    memset(tmpResult, 0, sizeof tmpResult);
    if( -1 == absResult.compResult )
        *retp++ = '0';
    int hasPoint = 0, tmpc = 0;
    sprintf(tmpUpNumber, "%.*s", absResult.bValidLen - 1, a);
    a += absResult.aValidLen < absResult.bValidLen - 1? absResult.aValidLen : absResult.bValidLen - 1;
    for( ; precision > 0; precision -= hasPoint ) {
        if( '\0' == *a && 0 == hasPoint )
            hasPoint = 1, *retp++ = '.';
        sprintf(tmpUpNumber, "%s%c", tmpUpNumber, '\0' == *a ? '0' : *a++);
        for( tmpc = 0, _getStrAbsStatus(tmpUpNumber, b, base, &absResult); tmpc < base && -1 != absResult.compResult; _getStrAbsStatus(tmpUpNumber, b, base, &absResult), ++tmpc )
            strMinus(tmpUpNumber, b, base, tmpUpNumber);
        *retp++ = 16 == base && tmpc > 9 ? tmpc - 10 + 'A' : tmpc + '0';
    }
    *retp = '\0';
    for( retp = tmpResult; '0' == *retp && '.' != *(retp + 1); ++retp )
        continue;
    sprintf(resultp, "%s", retp);
    return result;
}
static int trimPrefix_(char* a, int base, int* sign)
{
    char* ap = a;
    int signPre = 1;
    while( ' ' == *ap || '\t' == *ap )
        ++ap;
    if( '-' == *ap || '+' == *ap )
        signPre = '-' == *ap++ ? -1 : 1;
    while( ' ' == *ap || '\t' == *ap )
        ++ap;
    if( 16 == base && '0' == *ap && 'x' == (*(ap + 1) | 0x20) ) {
        ap += 2;
    }
    while( '0' == *ap )
        ++ap;
    if( '\0' == *ap && ap > a )
        --ap;
    *sign = signPre;
    return ap - a;
}
/*********************************************
	function 	: zyn_kmp 
	Description : 在s字符串中匹配p字符串,未找到返回NULL，否则返回第一个匹配到的字符串位置
	Author		: zhaoyining
	Date		: 2024-08-24
	History		: 
*********************************************/
char* zyn_kmp(const char* s, const int slen, const char* p, const int plen)
{
    if( NULL == s || slen < 1 || NULL == p || plen < 1 )
        return NULL;
    int pl = strlen(p) < plen ? strlen(p) : plen;
    if( pl < 1 )
        return s;
    int* next = (int*)malloc((pl > 1 ? pl : 2) * sizeof(int)), i = 0, j = 0;
    if( 0 != _getNext(p, pl, next) )
        return NULL;
    while( i < slen && j < pl )
        j = (-1 == j || s[i] == p[j]) && ++i ? j + 1 : next[j];
    free(next);
    next = NULL;
    return pl == j ? s + i - j : NULL;
}
/*********************************************
	function 	: 静态函数区
	Description : 本文件函数使用的功能函数
	Author		: zhaoyining
	Date		: 2024-08-24
	History		: 
*********************************************/
static int _getNext(const char* s, const int len, int* next)
{
    if( NULL == next )
        return -1;
    next[0] = -1, next[1] = 0;
    int i = 2, j = 0;
    while( i <= len )
        if( -1 == j || s[i - 1] == s[j] )
            next[i++] = ++j;
        else
            j = next[j];
    return 0;
}
static int _getStrAbsStatus(char* a, char* b, int base, STR_ABS_COMP_STATUS* result)
{
    if( NULL == a || NULL == b || base < 2 || NULL == result )
        return -1;
    int compResult = 0, aPrefix = 0, bPrefix = 0, aSign = 1, bSign = 1, aValidLen = 0, bValidLen = 0;
    a += aPrefix = trimPrefix_(a, base, &aSign);
    b += bPrefix = trimPrefix_(b, base, &bSign);
    char *ap = a, *bp = b;
    for( ; 0 == _strNumberCheck(*ap, base); ++aValidLen ) {
        ++ap;
    }
    if( '\0' != *ap )
        return -2;
    for( ; 0 == _strNumberCheck(*bp, base); ++bValidLen )
        ++bp;
    if( '\0' != *bp )
        return -3;
    if( aValidLen > bValidLen )
        compResult = 1;
    else if( aValidLen < bValidLen )
        compResult = -1;
    else
        for( ; '\0' != *a; ++a, ++b )
            if( *a != *b ) {
                compResult = *a > *b ? 1 : -1;
                break;
            }
    result->compResult = compResult;
    result->aSign = aSign;
    result->bSign = bSign;
    result->aPrefix = aPrefix;
    result->bPrefix = bPrefix;
    result->aValidLen = aValidLen;
    result->bValidLen = bValidLen;
    return 0;
}
static int _strNumberCheck(char c, int base)
{
    int ret = -1;
    switch( base ) {
        case 16:
            if( 'a' <= (c | 0x20) && (c | 0x20) <= 'f' )
                ret = 0;
            //fall through
        case 10:
            if( '8' <= c && c <= '9' )
                ret = 0;
            //fall through
        case 8:
            if( '2' <= c && c <= '7' )
                ret = 0;
            //fall through
        case 2:
            if( '0' <= c && c <= '1' )
                ret = 0;
            break;
        default:
            break;
    }
    return ret;
}
static char* _strPlus(char* a, char* b, int base, char* result)
{
    char tmpResult[STRING_NUMBER_MAX_LEN];
    memset(tmpResult, 0, sizeof tmpResult);
    int aLen = strlen(a), bLen = strlen(b), upbit = 0;
    int resultLen = aLen > bLen ? aLen : bLen;
    if( resultLen >= STRING_NUMBER_MAX_LEN - 1 ) // resultLen is location of tmp output, must before '\0'
        return NULL;
    for( --aLen, --bLen; resultLen >= 0; --aLen, --bLen, --resultLen ) {
        int tmpa = 0, tmpb = 0;
        if( aLen >= 0 && 16 == base && 'a' <= (a[aLen] | 0x20) && (a[aLen] | 0x20) <= 'f' )
            tmpa = (a[aLen] | 0x20) - 'a' + 10;
        else
            tmpa = aLen >= 0 ? a[aLen] - '0' : 0;
        if( bLen >= 0 && 16 == base && 'a' <= (b[bLen] | 0x20) && (b[bLen] | 0x20) <= 'f' )
            tmpb = (b[bLen] | 0x20) - 'a' + 10;
        else
            tmpb = bLen >= 0 ? b[bLen] - '0' : 0;
        int tmpSum = tmpa + tmpb + upbit;
        upbit = tmpSum >= base ? 1 : 0;
        tmpSum -= base * upbit;
        if( 16 == base && tmpSum > 9 )
            tmpResult[resultLen] = 'A' + tmpSum - 10;
        else
            tmpResult[resultLen] = '0' + tmpSum;
    }
    sprintf(result, "%s", tmpResult + ('0' == tmpResult[0] ? 1 : 0));
    return result;
}
static char* _strMinus(char* a, char* b, int base, char* result)
{
    char tmpResult[STRING_NUMBER_MAX_LEN];
    memset(tmpResult, 0, sizeof tmpResult);
    int aLen = strlen(a) - 1, bLen = strlen(b) - 1, upbit = 0;
    for( ; aLen >= 0; --aLen, --bLen ) {
        int tmpa = 0, tmpb = 0;
        if( aLen >= 0 && 16 == base && 'a' <= (a[aLen] | 0x20) && (a[aLen] | 0x20) <= 'f' )
            tmpa = (a[aLen] | 0x20) - 'a' + 10;
        else
            tmpa = aLen >= 0 ? a[aLen] - '0' : 0;
        if( bLen >= 0 && 16 == base && 'a' <= (b[bLen] | 0x20) && (b[bLen] | 0x20) <= 'f' )
            tmpb = (b[bLen] | 0x20) - 'a' + 10;
        else
            tmpb = bLen >= 0 ? b[bLen] - '0' : 0;

        tmpa -= upbit;
        upbit = tmpa < tmpb ? 1 : 0;
        int tmpc = tmpa + upbit * base - tmpb;
        tmpResult[aLen] = tmpc + (16 == base && tmpc > 9 ? 'A' - 10 : '0');
    }
    char* retp = tmpResult;
    while( '0' == *retp )
        ++retp;
    sprintf(result, "%s", '\0' == *retp ? "0" : retp);
    return result;
}
