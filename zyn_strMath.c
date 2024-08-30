/*********************************************
	File name	: zyn_strMath.c
	Description : personal string algorithm: huge number calculate function, match function
	Author		: zhaoyining
	Date		: 2022-07-05
	History		: 2022-07-05 creat file
	History		: 2024-08-24 add string KMP alog 
	History		: 2024-08-24 revise string plus and minus
	History		: 2024-08-26 revise string time
	History		: 2024-08-29 revise string divide
*********************************************/
#include "zynfunc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
static inline char _getChrVal(char c, int base);
static char* _strPlus(char* a, char* b, int base, char* result);
static char* _strMinus(char* a, char* b, int base, char* result);
static char* _strDivide( STR_ABS_COMP_RESULT st, int base, char* quot, char* rem);
static int _getNext(const char* s, const int len, int* next);
/*********************************************
	function 	: strPlus 
	Description : 字符串数字加法
	Author		: zhaoyining
	Date		: 2024-08-24
	History		: 
*********************************************/
char* strPlus(char* a, char* b, int base, char* result)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base);
    if( 1 != absRet.status )
        return NULL;
    char* max = 1 == absRet.cmpRet ? absRet.aVldP : absRet.bVldP, tmpRet[STRING_NUMBER_MAX_LEN] = "0";
    char* min = 1 != absRet.cmpRet ? absRet.aVldP : absRet.bVldP, * tp = tmpRet;
    char* sign = 1 == absRet.cmpRet && -1 == absRet.aSgn || -1 == absRet.cmpRet && -1 == absRet.bSgn ? "-" : "";
    if( !(0 == absRet.cmpRet && absRet.aSgn != absRet.bSgn) )
        absRet.aSgn == absRet.bSgn ? _strPlus(max, min, base, tp) : _strMinus(max, min, base, tp);
    for( ; '0' == *tp && '\0' != tp[1]; ++tp ) ;
    sprintf(result, "%s%s%s", sign, 16 == base ? "0x" : "", tp);
    return result;
}
/*********************************************
	function 	: strMinus
	Description : 字符串数字减法
	Author		: zhaoyining
	Date		: 2024-08-24
	History		: 
*********************************************/
char* strMinus(char* a, char* b, int base, char* result)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base);
    if( 1 != absRet.status )
        return NULL;
    char* max = 1 == absRet.cmpRet ? absRet.aVldP : absRet.bVldP, tmpRet[STRING_NUMBER_MAX_LEN] = "0";
    char* min = 1 != absRet.cmpRet ? absRet.aVldP : absRet.bVldP, * tp = tmpRet;
    char* sign = 1 == absRet.aSgn && 1 == absRet.bSgn && -1 == absRet.cmpRet
        || -1 == absRet.aSgn && (1 == absRet.bSgn || -1 == absRet.bSgn && 1 == absRet.cmpRet) ? "-" : "";
    if( !(0 == absRet.cmpRet && absRet.aSgn == absRet.bSgn) )
        absRet.aSgn != absRet.bSgn ? _strPlus(max, min, base, tp) : _strMinus(max, min, base, tp);
    for( ; '0' == *tp && '\0' != tp[1]; ++tp ) ;
    sprintf(result, "%s%s%s", sign, 16 == base ? "0x" : "", tp);
    return result;
}
/*********************************************
	function 	: strTime
	Description : 字符串数字乘法
	Author		: zhaoyining
	Date		: 2024-08-26
	History		: 
*********************************************/
char* strTime(char* a, char* b, int base, char* result)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base);
    if( 1 != absRet.status || absRet.aVldL + (absRet.aSgn != absRet.bSgn ? 1 : 0)
        + absRet.bVldL + (16 == base ? 2 : 0) >= STRING_NUMBER_MAX_LEN - 1 )
        return NULL;
    a = absRet.aVldP, b = absRet.bVldP;
    char tmpRet[STRING_NUMBER_MAX_LEN] = "0", *tp = tmpRet;
    if( !(0 == absRet.aVldL || 1 == absRet.aVldL && '0' == *absRet.aVldP
        ||  0 == absRet.bVldL || 1 == absRet.bVldL && '0' == *absRet.bVldP) ) {
        int tmpResult[STRING_NUMBER_MAX_LEN] = {0}, i = 0, j = 0;
        for( char tmpa = _getChrVal(a[i], base); i < absRet.aVldL; tmpa = _getChrVal(a[++i], base), j = 0 )
            for( char tmpb = _getChrVal(b[j], base); j < absRet.bVldL; tmpb = _getChrVal(b[++j], base) )
                tmpResult[i + 1 + j] += tmpa * tmpb;
        for( div_t tmp = div(tmpResult[i = absRet.aVldL - 1 + absRet.bVldL - 1 + 1], base);
                i >= 0; tmp = div(tmpResult[--i] + tmp.quot, base) )
            tp[i] = tmp.rem + (16 == base && tmp.rem > 9 ? 'A' - 10 : '0');
    }
    for( ; '0' == *tp && '\0' != tp[1]; ++tp ) ;
    sprintf(result, "%s%s%s", absRet.aSgn != absRet.bSgn ? "-" : "", 16 == base ? "0x" : "", tp);
    return result;
}
/*********************************************
	function 	: strDivide
	Description : 字符串数字除法
	Author		: zhaoyining
	Date		: 2024-08-29
	History		: 
*********************************************/
extern char* strDivide(char* a, char* b, int base, char* quot, char* rem)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base);
    if( 1 != absRet.status || NULL == quot || NULL == rem
        || 0 == absRet.bVldL || 1 == absRet.bVldL && '0' == *absRet.bVldP )
        return NULL;
    char tmpQuot[STRING_NUMBER_MAX_LEN] = "0", tmpRem[STRING_NUMBER_MAX_LEN] = "0", *tp = NULL;
    if( !(0 == absRet.aVldL || 1 == absRet.aVldL && '0' == *absRet.aVldP) )
        switch( absRet.cmpRet ) {
            case -1:
                sprintf(tmpQuot, "%s", absRet.aVldP);
                sprintf(tmpRem, "%s", absRet.bVldP);
                break;
            case 0:
                sprintf(tmpQuot, "1");
                sprintf(tmpRem, "0");
                break;
            default:
            case 1:
                _strDivide(absRet, base, tmpQuot, tmpRem);
                break;
        }
    for( tp = tmpQuot; '0' == *tp && '\0' != tp[1]; ++tp ) ;
    sprintf(quot, "%s%s%s", absRet.aSgn != absRet.bSgn ? "-" : "", 16 == base ? "0x" : "", tp);
    for( tp = tmpRem; '0' == *tp && '\0' != tp[1]; ++tp ) ;
    sprintf(rem, "%s%s%s", absRet.aSgn != absRet.bSgn ? "-" : "", 16 == base ? "0x" : "", tp);
    return quot;
}
/*********************************************
	function 	: strGcd
	Description : 字符串数字最大公约数
	Author		: zhaoyining
	Date		: 2024-08-29
	History		: 
*********************************************/
char* strGcd(char* a, char* b, int base, char* result)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base), st = {0};
    if( 1 != absRet.status || NULL == result
        || 0 == absRet.bVldL || 1 == absRet.bVldL && '0' == *absRet.bVldP
        || 0 == absRet.aVldL || 1 == absRet.aVldL && '0' == *absRet.aVldP )
        return NULL;
    char tmpQuot[STRING_NUMBER_MAX_LEN] = {'\0'}, tmpRem[STRING_NUMBER_MAX_LEN] = {'\0'}, * swap = NULL;
    char tmpa[STRING_NUMBER_MAX_LEN] = {'\0'}, tmpb[STRING_NUMBER_MAX_LEN] = {'\0'}, * ta = tmpa, * tb = tmpb;
    for( sprintf(ta, "%s", absRet.aVldP), sprintf(tb, "%s", absRet.bVldP), st = _getStrAbsStatus(ta, tb, base); ;
            swap = ta, ta = tb, tb = swap, st = _getStrAbsStatus(ta, tb, base) )
        if( 1 != st.status )
            return NULL;
        else if( st.cmpRet >= 1 ) {
            if( NULL == _strDivide(st, base, tmpQuot, tmpRem) )
                return NULL;
            for( swap = tmpRem; '0' == *swap; ++swap ) ;
            if( '\0' == *swap )
                break;
            sprintf(ta, "%s", tmpRem);
        }
    while( '0' == *tb && '\0' != tb[1] )
        ++tb;
    sprintf(result, "%s%s", 16 == base ? "0x" : "", tb);
    return result;
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
            if( (*a | 0x20) != (*b | 0x20) ) {
                ret.cmpRet = (*a | 0x20) > (*b | 0x20) ? 1 : -1;
                break;
            }
    return ret;
}
static char* _trimPrefix(char* a, int base, char* sign)
{
    char* ap = a;
    while( ' ' == *ap || '\t' == *ap )
        ++ap;
    *sign = '\0' == *ap || '-' == *ap || '+' == *ap ?  '-' == *ap++ ? -1 : 1 : 1;
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
    char tmpResult[STRING_NUMBER_MAX_LEN] = {'\0'}, upbit = 0;
    int aLen = strlen(a), bLen = strlen(b), resultLen = aLen > bLen ? aLen : bLen;
    if( resultLen >= STRING_NUMBER_MAX_LEN - 1 ) // resultLen is location of tmp output, must before '\0'
        return NULL;
    for( --aLen, --bLen; resultLen >= 0; --aLen, --bLen, --resultLen ) {
        char tmpa = aLen >= 0 ? _getChrVal(a[aLen], base) : 0, tmpb = bLen >= 0 ? _getChrVal(b[bLen], base) : 0, tmpSum = tmpa + tmpb + upbit;
        upbit = tmpSum >= base ? 1 : 0;
        tmpSum -= base * upbit;
        tmpResult[resultLen] = (16 == base && tmpSum > 9 ? 'A' - 10 : '0') + tmpSum;
    }
    sprintf(result, "%s", tmpResult + ('0' == tmpResult[0] ? 1 : 0));
    return result;
}
static char* _strMinus(char* a, char* b, int base, char* result)
{
    char upbit = 0;
    int aLen = strlen(a), bLen = strlen(b);
    if( (16 == base ? 2 : 0) + aLen >= STRING_NUMBER_MAX_LEN - 1 ) // resultLen is location of tmp output, must before '\0'
        return NULL;
    for( result[aLen--] = '\0', --bLen; aLen >= 0; --aLen, --bLen ) {
        char tmpa = aLen >= 0 ? _getChrVal(a[aLen], base) : 0, tmpb = bLen >= 0 ? _getChrVal(b[bLen], base) : 0;
        tmpa -= upbit;
        upbit = tmpa < tmpb ? 1 : 0;
        char tmpSum = tmpa + upbit * base - tmpb;
        result[aLen] = (16 == base && tmpSum > 9 ? 'A' - 10 : '0') + tmpSum;
    }
    return result;
}
static char* _strDivide( STR_ABS_COMP_RESULT st, int base, char* quot, char* rem)
{
    char* tmpNum = (char*)calloc((st.bVldL << 1) + 3, sizeof(char));
    if( NULL == tmpNum )
        return NULL;
    char quotVal = 0, *np = NULL, * qp = quot, * tmpRem = tmpNum + st.bVldL + 2;
    snprintf(tmpNum, st.bVldL + 1, "%s", st.aVldP);
    STR_ABS_COMP_RESULT numSt = {0}, remSt = {0};
    for( int i = st.bVldL; i <= st.aVldL; ) {
        numSt = _getStrAbsStatus(tmpNum, st.bVldP, base);
        if( 1 != numSt.status )
            break;
        if( numSt.cmpRet >= 0 && ++quotVal )
            _strMinus(tmpNum, st.bVldP, base, tmpRem);
        else
            sprintf(tmpRem, "%s", tmpNum);
        remSt = _getStrAbsStatus(tmpRem, st.bVldP, base);
        if( 1 != remSt.status )
            break;
        if( remSt.cmpRet < 0 ) {
            *qp++ = quotVal + (16 == base && quotVal > 9 ? 'A' - 10 : '0');
            quotVal = 0;
            sprintf(tmpNum, "%s%c", tmpRem + ('0' == *tmpRem ? 1 : 0), st.aVldP[i++]);
        }
        else
            sprintf(tmpNum, "%s", tmpRem);
    }
    sprintf(rem, "%s", tmpRem);
    free(tmpNum);
    tmpNum = NULL;
    if( 1 != numSt.status || 1 != remSt.status )
        return NULL;
    *qp = '\0';
    return quot;
}
static inline char _getChrVal(char c, int base)
{
    return (c | 0x20) - (16 == base && 'a' <= (c | 0x20) && (c | 0x20) <= 'f' ? 'a' - 10 : '0');
}
