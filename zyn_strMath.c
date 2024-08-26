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
    char cmpRet;
    char aSgn;
    char bSgn;
    char status;
    char* aVldP;
    char* bVldP;
    int aVldL;
    int bVldL;
} STR_ABS_COMP_RESULT;
static STR_ABS_COMP_RESULT _getStrAbsStatus(char* a, char* b, int base);
static char* _trimPrefix(char* a, int base, char* sign);
static int _strChrCheck(char c, int base);
static char* _strPlus(char* a, char* b, int base, char* result);
static char* _strMinus(char* a, char* b, int base, char* result);
static int _getNext(const char* s, const int len, int* next);
char* strPlus(char* a, char* b, int base, char* result)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base);
    if( 1 != absRet.status )
        return NULL;
    char* max = 1 == absRet.cmpRet ? absRet.aVldP : absRet.bVldP;
    char* min = 1 != absRet.cmpRet ? absRet.aVldP : absRet.bVldP, * ret = result;
    if( 0 == absRet.cmpRet && absRet.aSgn != absRet.bSgn )
        ret[0] = '0', ret[1] = '\0';
    else {
        if( 1 == absRet.cmpRet && -1 == absRet.aSgn || -1 == absRet.cmpRet && -1 == absRet.bSgn )
            *ret++ = '-';
        absRet.aSgn == absRet.bSgn ? _strPlus(max, min, base, ret) : _strMinus(max, min, base, ret);
    }
    return result;
}
char* strMinus(char* a, char* b, int base, char* result)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base);
    if( 1 != absRet.status )
        return NULL;
    char* max = 1 == absRet.cmpRet ? absRet.aVldP : absRet.bVldP;
    char* min = 1 != absRet.cmpRet ? absRet.aVldP : absRet.bVldP, * ret = result;
    if( 0 == absRet.cmpRet && absRet.aSgn == absRet.bSgn )
        ret[0] = '0', ret[1] = '\0';
    if( 1 == absRet.aSgn && 1 == absRet.bSgn && -1 == absRet.cmpRet
        || -1 == absRet.aSgn && (1 == absRet.bSgn || -1 == absRet.bSgn && 1 == absRet.cmpRet) )
        *ret++ = '-';
    absRet.aSgn != absRet.bSgn ? _strPlus(max, min, base, ret) : _strMinus(max, min, base, ret);
    return result;
}
#if 0
char* strTime(char* a, char* b, int base, char* result)
{
    char* retp = result;
    STR_ABS_COMP_RESULT absResult = {0, 0, 0, 0, 0};
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
    STR_ABS_COMP_RESULT absResult = {0, 0, 0, 0, 0};
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
#endif
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
static STR_ABS_COMP_RESULT _getStrAbsStatus(char* a, char* b, int base)
{
    STR_ABS_COMP_RESULT ret = {0, 0, 0, -1};
    if( NULL == a || NULL == b || base < 2 )
        return ret;
    a = ret.aVldP = _trimPrefix(a, base, &ret.aSgn);
    b = ret.bVldP = _trimPrefix(b, base, &ret.bSgn);
    for( ret.aVldL = 0, ret.status = -2; 0 == _strChrCheck(*a, base); ++ret.aVldL )
        ++a;
    if( '\0' != *a || ret.aVldL <= 0 )
        return ret;
    for( ret.bVldL = 0, ret.status = -3; 0 == _strChrCheck(*b, base); ++ret.bVldL )
        ++b;
    if( '\0' != *b || ret.bVldL <= 0 )
        return ret;
    ret.status = 1;
    if( ret.aVldL != ret.bVldL )
        ret.cmpRet = ret.aVldL > ret.bVldL ? 1 : -1;
    else
        for( ret.cmpRet = 0, a = ret.aVldP, b = ret.bVldP; '\0' != *a; ++a, ++b )
            if( *a != *b ) {
                ret.cmpRet = *a > *b ? 1 : -1;
                break;
            }
    return ret;
}
static char* _trimPrefix(char* a, int base, char* sign)
{
    char* ap = a;
    while( ' ' == *ap || '\t' == *ap )
        ++ap;
    *sign = '-' == *ap || '+' == *ap ?  '-' == *ap++ ? -1 : 1 : 1;
    while( ' ' == *ap || '\t' == *ap )
        ++ap;
    if( 16 == base && '0' == *ap && 'x' == (ap[1] | 0x20) )
        ap += 2;
    while( '0' == *ap || ' ' == *ap || '\t' == *ap )
        ++ap;
    if( '\0' == *ap && ap > a )
        --ap;
    return ap;
}
static int _strChrCheck(char c, int base)
{
    int ret = -1;
    switch( base ) {
        case 16: //fall through
            if( 'a' <= (c | 0x20) && (c | 0x20) <= 'f' )
                ret = 0;
        case 10: //fall through
            if( '8' <= c && c <= '9' )
                ret = 0;
        case 8: //fall through
            if( '2' <= c && c <= '7' )
                ret = 0;
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
    char tmpResult[STRING_NUMBER_MAX_LEN] = {'\0'};
    int aLen = strlen(a), bLen = strlen(b), resultLen = aLen > bLen ? aLen : bLen;
    if( resultLen >= STRING_NUMBER_MAX_LEN - 1 ) // resultLen is location of tmp output, must before '\0'
        return NULL;
    char upbit = 0;
    for( --aLen, --bLen; resultLen >= 0; --aLen, --bLen, --resultLen ) {
        char tmpa = 0, tmpb = 0;
        if( aLen >= 0 )
            tmpa = (a[aLen] | 0x20) - (16 == base && 'a' <= (a[aLen] | 0x20) && (a[aLen] | 0x20) <= 'f' ? 'a' - 10 : '0');
        if( bLen >= 0 )
            tmpb = (b[bLen] | 0x20) - (16 == base && 'a' <= (b[bLen] | 0x20) && (b[bLen] | 0x20) <= 'f' ? 'a' - 10 : '0');
        char tmpSum = tmpa + tmpb + upbit;
        upbit = tmpSum >= base ? 1 : 0;
        tmpSum -= base * upbit;
        tmpResult[resultLen] = (16 == base && tmpSum > 9 ? 'A' - 10 : '0') + tmpSum;
    }
    sprintf(result, "%s%s", 16 == base ? "0x" : "", tmpResult + ('0' == tmpResult[0] ? 1 : 0));
    return result;
}
static char* _strMinus(char* a, char* b, int base, char* result)
{
    char tmpResult[STRING_NUMBER_MAX_LEN] = {'\0'};
    int aLen = strlen(a), bLen = strlen(b);
    if( (16 == base ? 2 : 0) + aLen >= STRING_NUMBER_MAX_LEN - 1 ) // resultLen is location of tmp output, must before '\0'
        return NULL;
    char upbit = 0;
    for( --aLen, --bLen; aLen >= 0; --aLen, --bLen ) {
        char tmpa = 0, tmpb = 0;
        if( aLen >= 0 )
            tmpa = (a[aLen] | 0x20) - (16 == base && 'a' <= (a[aLen] | 0x20) && (a[aLen] | 0x20) <= 'f' ? 'a' - 10 : '0');
        if( bLen >= 0 )
            tmpb = (b[bLen] | 0x20) - (16 == base && 'a' <= (b[bLen] | 0x20) && (b[bLen] | 0x20) <= 'f' ? 'a' - 10 : '0');
        tmpa -= upbit;
        upbit = tmpa < tmpb ? 1 : 0;
        char tmpSum = tmpa + upbit * base - tmpb;
        tmpResult[aLen] = (16 == base && tmpSum > 9 ? 'A' - 10 : '0') + tmpSum;
    }
    char* retp = tmpResult;
    while( '0' == *retp )
        ++retp;
    sprintf(result, "%s%s", 16 == base ? "0x" : "", '\0' == *retp ? "0" : retp);
    return result;
}
