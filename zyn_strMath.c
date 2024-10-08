/*********************************************
    File name   : zyn_strMath.c
    Description : personal string algorithm: huge number calculate function, match function
    Author      : zhaoyining
    Date        : 2022-07-05
    History     : 2022-07-05 creat file
    History     : 2024-08-24 add string KMP alog 
    History     : 2024-08-24 revise string plus and minus
    History     : 2024-08-26 revise string time
    History     : 2024-08-29 revise string divide
    History     : 2024-09-03 add strBaseCvt function
*********************************************/
#include "zyn_func.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXBASE 36
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
static char _getChrStr(char v, int base);
static char* _generalBaseCvt(const char* a, int srcBase, int dstBase, char* result);
static char* _sliceBaseCvt(const char* a, int srcBase, int dstBase, int ratio, char* result);
static char* _hexOctBaseCvt(const char* a, int srcBase, int dstBase, char* result);
static char* _spilitByteTo3(char* lftBit, char* lftVal, char* result);
static char* _merge3ToByte(char* lftBit, char* lftVal, char* result);
static int _getBaseRelation(int srcBase, int dstBase);
static char* _strPlus(char* a, char* b, int base, char* result);
static char* _strMinus(char* a, char* b, int base, char* result);
static char* _strDivide( STR_ABS_COMP_RESULT st, int base, char* quot, char* rem);
static int _getNext(const char* s, const int len, int* next);
/*********************************************
    function    : strPlus 
    Description : 字符串数字加法
    Author      : zhaoyining
    Date        : 2024-08-24
    History     : 
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
    function    : strMinus
    Description : 字符串数字减法
    Author      : zhaoyining
    Date        : 2024-08-24
    History     : 
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
    function    : strTime
    Description : 字符串数字乘法
    Author      : zhaoyining
    Date        : 2024-08-26
    History     : 
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
            tp[i] = _getChrStr(tmp.rem, base);
    }
    for( ; '0' == *tp && '\0' != tp[1]; ++tp ) ;
    sprintf(result, "%s%s%s", absRet.aSgn != absRet.bSgn ? "-" : "", 16 == base ? "0x" : "", tp);
    return result;
}
/*********************************************
    function    : strDivide
    Description : 字符串数字除法
    Author      : zhaoyining
    Date        : 2024-08-29
    History     : 
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
    function    : strGcd
    Description : 字符串数字最大公约数
    Author      : zhaoyining
    Date        : 2024-08-29
    History     : 
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
    function    : strLcm
    Description : 字符串数字最小公倍数
    Author      : zhaoyining
    Date        : 2024-08-31
    History     : 
*********************************************/
char* strLcm(char* a, char* b, int base, char* result)
{
    STR_ABS_COMP_RESULT absRet = _getStrAbsStatus(a, b, base), st = {0};
    if( 1 != absRet.status || NULL == result
        || 0 == absRet.bVldL || 1 == absRet.bVldL && '0' == *absRet.bVldP
        || 0 == absRet.aVldL || 1 == absRet.aVldL && '0' == *absRet.aVldP )
        return NULL;
    char tmpGcd[STRING_NUMBER_MAX_LEN] = {'\0'}, tmpMid[STRING_NUMBER_MAX_LEN] = {'\0'}, ign[190] = {'\0'};
    if( NULL == strGcd(a, b, base, tmpGcd) || '0' == *tmpGcd && '\0' == tmpGcd[1] )
        return NULL;
    if( NULL == strDivide(a, tmpGcd, base, tmpMid, ign) || NULL == strTime(b, tmpMid, base, result) )
        return NULL;
    return '0' == *ign && '\0' == ign[1] ? result : NULL;
}
/*********************************************
    function    : strPow 
    Description : 返回a中的字符串数字的幂运算结果，进制base有效范围 2 - 36, x有效范围 >= 0
    Author      : zhaoyining
    Date        : 2024-09-05
    History     : 
*********************************************/
char* strPow(char* a, int base, int x, char* result)
{
    if( NULL == a || base < 2 || base > MAXBASE || x < 0 || NULL == result )
        return NULL;
    if( '0' == *a && '\0' == a[1] )
        sprintf(result, "0");
    else if( 0 == x || '1' == *a && '\0' == a[1] )
        sprintf(result, "1");
    else {
        char tmpa[STRING_NUMBER_MAX_LEN] = "", tmpb[STRING_NUMBER_MAX_LEN] = "", * ap = tmpa, * bp = tmpb, *swap = NULL;
        char tmpRet[STRING_NUMBER_MAX_LEN] = "1", tmpMid[STRING_NUMBER_MAX_LEN] = "", * rp = tmpRet, * mp = tmpMid;
        sprintf(ap, "%s", a);
        for( ; 0 != x; x >>= 1 ) {
            STR_ABS_COMP_RESULT st = {0};
            if( 1 == (x & 1) ) {
                st = _getStrAbsStatus(rp, ap, base);
                if( 1 != st.status || st.aVldL + st.bVldL >= STRING_NUMBER_MAX_LEN )
                    return NULL;
                if( NULL == strTime(rp, ap, base, mp) )
                    return NULL;
                swap = mp, mp = rp, rp = swap;
                if( 0 == x >> 1 )
                    break;
            }
            st = _getStrAbsStatus(ap, ap, base);
            if( 1 != st.status || st.aVldL + st.bVldL >= STRING_NUMBER_MAX_LEN )
                return NULL;
            if( NULL == strTime(ap, ap, base, bp) )
                return NULL;
            swap = ap, ap = bp, bp = swap;
        }
        sprintf(result, "%s", rp);
    }
    return result;
}
/*********************************************
    function    : strBaseCvt
    Description : 将a中的字符串数字由srcBase进制转换为dstBase进制，结果保存在result数组中，异常返回NULL
                  srcBase和dstBase有效范围 2 - 36
    Author      : zhaoyining
    Date        : 2024-09-03
    History     : 
*********************************************/
extern char* strBaseCvt(const char* a, int srcBase, int dstBase, char* result)
{
    if( NULL == a || srcBase < 2 || srcBase > 36 || dstBase < 2 || dstBase > 36 || NULL == result )
        return NULL;
    while( ' ' == *a || '-' == *a || '+' == *a || '\t' == *a )
        ++a;
    int bs = _getBaseRelation(srcBase, dstBase);
    char* ret = result;
    switch( bs ) {
        case 0:
            sprintf(result, "%s", a);
            break;
        case 2: case 3: case 4: case 5:
        case -2: case -3: case -4: case -5:
             ret = _sliceBaseCvt(a, srcBase, dstBase, bs, result);
            break;
        case -6: case 6:
             ret = _hexOctBaseCvt(a, srcBase, dstBase, result);
            break;
        default:
            ret = _generalBaseCvt(a, srcBase, dstBase, result);
            break;
    }
    return ret;
}
/*********************************************
    function    : zyn_kmp 
    Description : 在s字符串中匹配p字符串,未找到返回NULL，否则返回第一个匹配到的字符串位置
    Author      : zhaoyining
    Date        : 2024-08-24
    History     : 
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
    function    : 静态函数区
    Description : 本文件函数使用的功能函数
    Author      : zhaoyining
    Date        : 2024-08-24
    History     : 
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
    if( NULL == a || NULL == b || base < 2 || base > MAXBASE )
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
    for( ap += 16 == base && '0' == *ap && 'x' == (ap[1] | 0x20) ? 2 : 0; '0' == *ap || ' ' == *ap || '\t' == *ap; ++ap) ;
    return '\0' == *ap && ap > a ? ap - 1 : ap;
}
static char* _strPlus(char* a, char* b, int base, char* result)
{
    char tmpResult[STRING_NUMBER_MAX_LEN] = {'\0'}, upbit = 0;
    int aLen = strlen(a), bLen = strlen(b), resultLen = aLen > bLen ? aLen : bLen;
    if( resultLen >= STRING_NUMBER_MAX_LEN - 1 ) // resultLen is location of tmp output, must before '\0'
        return NULL;
    for( --aLen, --bLen; resultLen >= 0; --aLen, --bLen, --resultLen ) {
        char tmpa = aLen >= 0 ? _getChrVal(a[aLen], base) : 0;
        char tmpb = bLen >= 0 ? _getChrVal(b[bLen], base) : 0, tmpSum = tmpa + tmpb + upbit;
        upbit = tmpSum >= base ? 1 : 0;
        tmpSum -= base * upbit;
        tmpResult[resultLen] = _getChrStr(tmpSum, base);
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
        result[aLen] = _getChrStr(tmpSum, base);
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
        numSt.cmpRet >= 0 && ++quotVal ?  _strMinus(tmpNum, st.bVldP, base, tmpRem) : sprintf(tmpRem, "%s", tmpNum);
        remSt = _getStrAbsStatus(tmpRem, st.bVldP, base);
        if( 1 != remSt.status )
            break;
        if( remSt.cmpRet < 0 ) {
            *qp++ =  _getChrStr(quotVal, base);
            quotVal = 0;
            sprintf(tmpNum, "%s%c", tmpRem + ('0' == *tmpRem ? 1 : 0), st.aVldP[i++]);
        }
        else
            sprintf(tmpNum, "%s", tmpRem);
    }
    sprintf(rem, "%s", tmpRem);
    *qp = '\0';
    free(tmpNum);
    tmpNum = NULL;
    if( 1 != numSt.status || 1 != remSt.status )
        return NULL;
    return quot;
}
static int _getBaseRelation(int srcBase, int dstBase)
{
    int ret = 0;
    if( srcBase == dstBase )
        ret = 0;
    else if( srcBase > dstBase )
        if( srcBase == dstBase * dstBase )
            ret = 2;
        else if( srcBase == dstBase * dstBase * dstBase )
            ret = 3;
        else if( srcBase == dstBase * dstBase * dstBase * dstBase )
            ret = 4;
        else if( srcBase == dstBase * dstBase * dstBase * dstBase * dstBase )
            ret = 5;
        else if( 16 == srcBase && 8 == dstBase )
            ret = 6;
        else
            ret = 1;
    else
        if( srcBase * srcBase == dstBase )
            ret = -2;
        else if( srcBase * srcBase * srcBase == dstBase )
            ret = -3;
        else if( srcBase * srcBase * srcBase * srcBase == dstBase )
            ret = -4;
        else if( srcBase * srcBase * srcBase * srcBase * srcBase == dstBase )
            ret = -5;
        else if( 8 == srcBase && 16 == dstBase )
            ret = -6;
        else
            ret = -1;
    return ret;
}
static char* _hexOctBaseCvt(const char* a, int srcBase, int dstBase, char* result)
{
    char tmpRet0[STRING_NUMBER_MAX_LEN] = {0}, tmpRet1[STRING_NUMBER_MAX_LEN] = {0}, * r0p = tmpRet0, * r1p = tmpRet1;
    char tmpRet2[STRING_NUMBER_MAX_LEN] = {0}, tmpRet3[STRING_NUMBER_MAX_LEN] = {0}, * r2p = tmpRet2, * r3p = tmpRet3;
    char* rp = NULL;
    int r0LeftVal = 0, r1LeftVal = 0, r2LeftVal = 0, r3LeftVal = 0, r0LeftBit = 0, r3LeftBit = 3;
    int r1LeftBit = srcBase > dstBase ? 2 : 1, r2LeftBit = srcBase > dstBase ? 1 : 2;
        if( srcBase > dstBase )
            while( 0 == _strChrCheck(*a, 16) ) {
                int curVal = _getChrVal(*a++, srcBase) & 0xF;
                r1LeftVal = r1LeftVal << 4 & 0x70 | curVal;
                r2LeftVal = r2LeftVal << 4 & 0x70 | curVal;
                r0LeftVal = r0LeftVal << 4 & 0x70 | curVal;
                r1LeftBit += 4, r2LeftBit += 4, r0LeftBit += 4;
                r1p = _spilitByteTo3(&r1LeftBit, &r1LeftVal, r1p);
                r2p = _spilitByteTo3(&r2LeftBit, &r2LeftVal, r2p);
                r0p = _spilitByteTo3(&r0LeftBit, &r0LeftVal, r0p);
            }
        else
            while( 0 == _strChrCheck(*a, 8) ) {
                int curVal = _getChrVal(*a++, srcBase) & 0x7;
                r1LeftVal = r1LeftVal << 3 & 0x38 | curVal;
                r2LeftVal = r2LeftVal << 3 & 0x38 | curVal;
                r3LeftVal = r3LeftVal << 3 & 0x38 | curVal;
                r0LeftVal = r0LeftVal << 3 & 0x38 | curVal;
                r1LeftBit += 3, r2LeftBit += 3, r3LeftBit += 3, r0LeftBit += 3;
                r1p = _merge3ToByte(&r1LeftBit, &r1LeftVal, r1p);
                r2p = _merge3ToByte(&r2LeftBit, &r2LeftVal, r2p);
                r3p = _merge3ToByte(&r3LeftBit, &r3LeftVal, r3p);
                r0p = _merge3ToByte(&r0LeftBit, &r0LeftVal, r0p);
            }
        if( 0 == r3LeftBit )
            rp = tmpRet3;
        else if( 0 == r2LeftBit )
            rp = tmpRet2;
        else if( 0 == r1LeftBit )
            rp = tmpRet1;
        else
            rp = tmpRet0;
    sprintf(result, "%s", rp);
    return result;
}
static char* _generalBaseCvt(const char* a, int srcBase, int dstBase, char* result)
{
    char tmpNum[STRING_NUMBER_MAX_LEN] = {0}, tmpQuot[STRING_NUMBER_MAX_LEN] = {0}, tmpRem[STRING_NUMBER_MAX_LEN] = {0};
    char* tqp = tmpQuot, * trp = tmpRem, * rp = result;
    for( int numSlice = 0; ; a = tmpNum, numSlice = 0, tqp = tmpQuot ) {
        div_t d = {0};
        while( 0 == _strChrCheck(*a, srcBase) ) {
            numSlice = numSlice * srcBase + _getChrVal(*a++, srcBase);
            d = div(numSlice, dstBase);
            *tqp++ = _getChrStr(d.quot, srcBase);
            numSlice = d.rem;
        }
        *tqp = '\0';
        *trp++ = _getChrStr(numSlice, dstBase);
        sprintf(tmpNum, "%s", tmpQuot);
        STR_ABS_COMP_RESULT st = _getStrAbsStatus(tmpNum, "0", srcBase);
        if( 1 != st.status )
            return NULL;
        else if( 0 == st.cmpRet )
            break;
    }
    for( --trp; trp >= tmpRem; *rp++ = *trp-- ) ;
    *rp = '\0';
}
static char* _sliceBaseCvt(const char* a, int srcBase, int dstBase, int ratio, char* result)
{
    char* rp = result;
    if( ratio > 0 )
        for( ; 0 == _strChrCheck(*a, srcBase); rp += ratio ) {
            div_t d = {_getChrVal(*a++, srcBase), 0};
            for( int loc = ratio - 1; loc >= 0; --loc ) {
                d = div(d.quot, dstBase);
                rp[loc] = _getChrStr(d.rem, dstBase);
            }
        }
    else {
        ratio = -ratio;
        int len = 0, v = 0;
        for( char* ap = a; '\0' != *ap++; ++len ) ;
        for( int i = 0, width = len % ratio == 0 ? ratio : len % ratio; i < (len + ratio - 1) / ratio; ++i, v = 0, width = ratio ) {
            for( int j = 0; j < width; ++j )
                v = v * srcBase + _getChrVal(*a++, srcBase);
            *rp++ = _getChrStr(v, dstBase);
        }
    }
    *rp = '\0';
    return result;
}
static char* _spilitByteTo3(char* lftBit, char* lftVal, char* result)
{
    char* ret = result + 1;
    switch( *lftBit ) {
        case 4:
            *result = _getChrStr(*lftVal >> 1 & 7, 8);
            *lftVal &= 1;
            break;
        case 5:
            *result = _getChrStr(*lftVal >> 2 & 7, 8);
            *lftVal &= 3;
            break;
        default: case 6:
            *result = _getChrStr(*lftVal >> 3 & 7, 8);
            result[1] = _getChrStr(*lftVal & 7, 8);
            *lftVal = 0;
            *lftBit -= 3;
            ret += 1;
            break;
    }
    *lftBit -= 3;
    return ret;
}
static char* _merge3ToByte(char* lftBit, char* lftVal, char* result)
{
    char* ret = result;
    switch( *lftBit ) {
        case 4:
            *ret++ = _getChrStr(*lftVal & 0xF, 16);
            *lftBit = 0;
            *lftVal = 0;
            break;
        case 5:
            *ret++ = _getChrStr(*lftVal >> 1 & 0xF, 16);
            *lftBit = 1;
            *lftVal &= 1;
            break;
        case 6:
            *ret++ = _getChrStr(*lftVal >> 2 & 0xF, 16);
            *lftBit = 2;
            *lftVal &= 3;
            break;
        default: case 3:
            *lftVal &= 7;
            break;
    }
    return ret;
}
static const char s_chr[38] = "0123456789abcdefghijklmnopqrstuvwxyz{";
static inline char _getChrVal(char c, int base)
{
    if( base < 2 || base > MAXBASE || base < 11 && (c < '0' || '9' < c)
        || base > 10 && !('0' <= c && c <= '9' || 'a' <= (c | 0x20) && (c | 0x20) < s_chr[base]) )
        return 0;
    return (c | 0x20) - ( base > 10 && (c | 0x20) >= 'a' ? 'a' - 10 : '0');
}
static int _strChrCheck(char c, int base)
{
    if( base < 2 || base > MAXBASE )
        return -1;
    for( int i = 0; i < base; ++i )
        if( (c | (i > 9 ? 0x20 : 0)) == s_chr[i] )
            return 0;
    return -1;
}
static char _getChrStr(char v, int base)
{
    return base < 2 || base > MAXBASE || v >= base ? 0 : (s_chr[v] ^ (v > 9 ? 0x20 : 0));
}
