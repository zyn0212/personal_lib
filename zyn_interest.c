/*********************************************
	File name	: zyn_maths.c
	Description     : personal math function
	Author		: zhaoyining
	Date		: 2022-07-05
	History		: 2022-07-05 creat file with fillTrangl function
*********************************************/
#include "zynfunc.h"
#include <stdio.h>
#define MAX_CONVERT_LENGTH 500
/*********************************************
	function 	: fillTrangl
	Description     : 按照顺时针和递增的顺序填充一个正方形螺旋矩阵，返回最后的填充数字
        Parameter       : startnum 矩阵的起始数字，trangeWide 矩阵宽度, array 数组地址(如果是二维数组需要强制转换),arrayWide 数组的行宽度
                  corner起始填充角: 1 右上， 2 右下， 3 左下， 其他 左上
        Author		: zhaoyining
	Date		: 2022-07-05
	History		: 2022-07-05
*********************************************/
int fillTrangl(int startnum, int tranglWide, int* array, int arrayWide, int corner)
{
    if( NULL == array || tranglWide < 1 || arrayWide < tranglWide )
        return startnum;
    if( 1 == tranglWide )
        return (*array = startnum) + 1;
    int* tp = array;
    int direction[4] = { 1, arrayWide, -1, -1 * arrayWide}, loopSize = 0, loopCount = 0, i = 0;
    switch( corner ) {
        default:
        case 0:
            corner = 0;
            break;
        case 3:
        case 2:
        case 1:
            loopCount += (tranglWide - 1) * corner;
            break;
    }
    for( loopSize = tranglWide - 1; i < tranglWide * tranglWide; ++i ) {
        *tp = startnum++;
        tp += direction[loopCount / loopSize];
        loopCount = ++loopCount >=  loopSize << 2 ? 0 : loopCount;
        if( loopCount == corner * loopSize ) {
            switch( corner ) {
                default:
                case 0:
                    tp += arrayWide + 1;
                    break;
                case 1:
                    tp += arrayWide - 1;
                    break;
                case 2:
                    tp -= arrayWide + 1;
                    break;
                case 3:
                    tp -= arrayWide - 1;
                    break;
            }
            loopSize = 0 == loopSize - 2 ? 1 : loopSize - 2;
            loopCount = loopSize * corner;
        }
    }
    return --startnum;
}
static int convertHex2Oct_(int hex, char* result)
{
    result[0] = (hex >> 9 & 0x7) + '0';
    result[1] = (hex >> 6 & 0x7) + '0';
    result[2] = (hex >> 3 & 0x7) + '0';
    result[3] = (hex & 0x7) + '0';
    return 4;
}
/*********************************************
	function 	: convertHex2Oct
	Description     : 将十六进制字符串转换成八进制字符串
        Parameter       : hex 是输入的十六进制字符串，result是转换后的八进制字符串，函数在输入的字符串中遇到非十六进制字符（除0～9、a～f、A～F之外的字符）后转换结束，包括前缀的空格
        Author		: zhaoyining
	Date		: 2022-07-13
	History		: 2022-07-13
*********************************************/
int convertHex2Oct(char* hex, char* result)
{
    if( NULL == hex || NULL == result )
        return -1;
    int convBuf = 0, single0Len = 0, single1Len = 0, single2Len = 0, count = 0;
    char tmpResult[3][MAX_CONVERT_LENGTH];
    memset(tmpResult, 0, sizeof tmpResult);
    while( 1 ) {
        if( '0' <= *hex && *hex <= '9' || 'a' <= (*hex | 0x20) && (*hex | 0x20) <= 'f' )
            convBuf = convBuf << 4 | (*hex & 0xF) + (*hex >> 6 & 1) * 9;
        else
            break;
        if( single0Len + 4 > MAX_CONVERT_LENGTH - 1 || single1Len + 4 > MAX_CONVERT_LENGTH - 1 || single2Len + 4 > MAX_CONVERT_LENGTH - 1 )
            return -1;
        switch( ++count % 3 ) {
            default:
            case 0:
                single0Len += convertHex2Oct_(convBuf, tmpResult[0] + single0Len);
                break;
            case 1:
                single1Len += convertHex2Oct_(convBuf, tmpResult[1] + single1Len);
                break;
            case 2:
                single2Len += convertHex2Oct_(convBuf, tmpResult[2] + single2Len);
                break;
        }
        ++hex;
    }
    char* rp = tmpResult[count % 3];
    while( '0' == *rp )
        ++rp;
    return sprintf(result, "%s", rp);
}
