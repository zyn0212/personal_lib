#include "zynfunc.h"
#include <stdio.h>
#include <string.h>
typedef struct {
    int absCompResult;
    int aSign;
    int aPrefix;
    int aValidLen;
    int bSign;
    int bPrefix;
    int bValidLen;
} STR_ABS_COMP_STATUS;
static int getStrAbsStatus_(char* a, char* b, int base, STR_ABS_COMP_STATUS* result);
static int strNumberCheck(char c, int base);
static char* strPlus_(char* a, char* b, int base, char* result);
static char* strMinus_(char* a, char* b, int base, char* result);
char* strPlus(char* a, char* b, int base, char* result)
{
    if( NULL == a || NULL == b || base < 2 || NULL == result ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    char* ret = result;
    STR_ABS_COMP_STATUS absResult = {0, 0, 0, 0, 0};
    if( 0 != getStrAbsStatus_(a, b, base, &absResult) ) {
        printf("input string contain unsupport character base on %d!\n", base);
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    if( absResult.aSign == absResult.bSign ) {
        if( -1 == absResult.aSign )
            *ret++ = '-';
        strPlus_(a, b, base, ret);
    }
    else if( -1 == absResult.aSign ) {
        if( 1 == absResult.absCompResult )
            *ret++ = '-';
        1 == absResult.absCompResult ? strMinus_(a, b, base, ret) : strMinus_(b, a, base, ret);
    }
    else {
        if( -1 == absResult.absCompResult )
            *ret++ = '-';
        -1 == absResult.absCompResult ? strMinus_(b, a, base, ret) : strMinus_(a, b, base, ret);
    }
    return result;
}
char* strMinus(char* a, char* b, int base, char* result)
{
    if( NULL == a || NULL == b || base < 2 || NULL == result ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    char* ret = result;
    STR_ABS_COMP_STATUS absResult = {0, 0, 0, 0, 0};
    if( 0 != getStrAbsStatus_(a, b, base, &absResult) ) {
        printf("input string contain unsupport character base on %d!\n", base);
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    if( absResult.aSign != absResult.bSign ) {
        if( -1 == absResult.aSign )
            *ret++ = '-';
        strPlus_(a, b, base, ret);
    }
    else if( -1 == absResult.aSign ) {
        if( 1 == absResult.absCompResult )
            *ret++ = '-';
        1 == absResult.absCompResult ? strMinus_(a, b, base, ret) : strMinus_(b, a, base, ret);
    }
    else {
        if( -1 == absResult.absCompResult )
            *ret++ = '-';
        -1 == absResult.absCompResult ? strMinus_(b, a, base, ret) : strMinus_(a, b, base, ret);
    }
    return result;
}
char* strTime(char* a, char* b, int base, char* result)
{
    if( NULL == a || NULL == b || base < 2 || NULL == result ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    char* retp = result;
    STR_ABS_COMP_STATUS absResult = {0, 0, 0, 0, 0};
    if( 0 != getStrAbsStatus_(a, b, base, &absResult) ) {
        printf("input string contain unsupport character base on %d!\n", base);
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    if( absResult.aValidLen + absResult.bValidLen >= STRING_NUMBER_MAX_LEN - 1 )
        return NULL;
    if( absResult.aSign != absResult.bSign )
        *retp++ = '-';
    int aLen = 0, bLen = 0, resLen = 0, tmpResult[STRING_NUMBER_MAX_LEN];
    memset(tmpResult, 0, sizeof tmpResult);
    for( ; aLen < absResult.aValidLen; ++aLen )
        for( bLen = 0; bLen < absResult.bValidLen; ++bLen ) {
            int tmpa = 0, tmpb = 0;
            tmpa = 16 == base && 'a' <= (a[aLen] | 0x20) && (a[aLen] | 0x20) <= 'f' ? (a[aLen] | 0x20) - 'a' + 10 : a[aLen] - '0';
            tmpb = 16 == base && 'a' <= (b[bLen] | 0x20) && (b[bLen] | 0x20) <= 'f' ? (b[bLen] | 0x20) - 'a' + 10 : b[bLen] - '0';
            tmpResult[aLen + bLen] += tmpa * tmpb;
            resLen = resLen < aLen + bLen ? aLen + bLen : resLen;
        }
    int upbit = 0;
    for( result[resLen + 1] = '\0'; resLen >= 0; --resLen ) {
        int tmpc = (tmpResult[resLen] + upbit) % base;
        upbit = (tmpResult[resLen] + upbit) / base;
        result[resLen] = 16 == base && tmpc > 9 ? tmpc - 10 + 'A' : tmpc + '0';
    }
    return result;

}
char* strDivide(char* a, char* b, int base, int precision, char* result)
{
    if( NULL == a || NULL == b || base < 2 || precision < 1 || precision > 200 || NULL == result ) {
        printf("fatal error: meet error in parameters!\n");
        return NULL;
    }
    STR_ABS_COMP_STATUS absResult = {0, 0, 0, 0, 0};
    if( 0 != getStrAbsStatus_(a, b, base, &absResult) ) {
        printf("input string contain unsupport character base on %d!\n", base);
        return NULL;
    }
    a += absResult.aPrefix;
    b += absResult.bPrefix;
    char tmpUpNumber[STRING_NUMBER_MAX_LEN], tmpResult[STRING_NUMBER_MAX_LEN];
    char* retp = tmpResult;
    memset(tmpResult, 0, sizeof tmpResult);
    if( absResult.aSign != absResult.bSign )
        *retp++ = '-';
    if( -1 == absResult.absCompResult )
        *retp++ = '0';
    int hasPoint = 0, tmpc = 0;
    sprintf(tmpUpNumber, "%.*s", absResult.bValidLen - 1, a);
    a += absResult.aValidLen < absResult.bValidLen - 1? absResult.aValidLen : absResult.bValidLen - 1;
    for( ; precision > 0; precision -= hasPoint ) {
        if( '\0' != *a )
            sprintf(tmpUpNumber, "%s%c", tmpUpNumber, *a++);
        else {
            sprintf(tmpUpNumber, "%s0", tmpUpNumber);
            if( 0 == hasPoint ) {
                *retp++ = '.';
                hasPoint = 1;
            }
        }
        for( tmpc = 0, getStrAbsStatus_(tmpUpNumber, b, base, &absResult); tmpc < base && -1 != absResult.absCompResult; getStrAbsStatus_(tmpUpNumber, b, base, &absResult), ++tmpc )
            strMinus(tmpUpNumber, b, base, tmpUpNumber);
        *retp++ = 16 == base && tmpc > 9 ? tmpc - 10 + 'A' : tmpc + '0';
    }
    *retp = '\0';
    retp = tmpResult;
    if( '-' == *retp ) {
        *result++ = '-';
        ++retp;
    }
    while( '0' == *retp && '.' != *(retp + 1) )
        ++retp;
    sprintf(result, "%s", retp);
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
static int getStrAbsStatus_(char* a, char* b, int base, STR_ABS_COMP_STATUS* result)
{
    int absCompResult = 0, aPrefix = 0, bPrefix = 0, aSign = 1, bSign = 1, aValidLen = 0, bValidLen = 0;
    a += aPrefix = trimPrefix_(a, base, &aSign);
    b += bPrefix = trimPrefix_(b, base, &bSign);
    char *ap = a, *bp = b;
    for( ; 0 == strNumberCheck(*ap, base); ++aValidLen ) {
        ++ap;
    }
    if( '\0' != *ap )
        return -2;
    for( ; 0 == strNumberCheck(*bp, base); ++bValidLen )
        ++bp;
    if( '\0' != *bp )
        return -3;
    if( aValidLen > bValidLen )
        absCompResult = 1;
    else if( aValidLen < bValidLen )
        absCompResult = -1;
    else
        for( ; '\0' != *a; ++a, ++b )
            if( *a != *b ) {
                absCompResult = *a > *b ? 1 : -1;
                break;
            }
    result->absCompResult = absCompResult;
    result->aSign = aSign;
    result->bSign = bSign;
    result->aPrefix = aPrefix;
    result->bPrefix = bPrefix;
    result->aValidLen = aValidLen;
    result->bValidLen = bValidLen;
    return 0;
}
static int strNumberCheck(char c, int base)
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
static char* strPlus_(char* a, char* b, int base, char* result)
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
static char* strMinus_(char* a, char* b, int base, char* result)
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
