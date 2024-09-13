/*********************************************
	File name	: zyn_interest.c
	Description : personal insterest function
	Author		: zhaoyining
	Date		: 2022-07-05
	History		: 2022-07-05 creat file with fillTrangl function
	              2024-07-04 add zyn_cvtDecToX function
    Histroy     : delete  convertHex2Oct function
    Histroy     : delete  zyn_cvtDecToX function
    Histroy     : 2024-09-11 add magicSquare
*********************************************/
#include "zyn_func.h"
#include <stdio.h>
#define MAX_CONVERT_LENGTH 500
static int _fillOddSq(int* arr, int arrWide, int sqWide, int start);
static int _fillEvenSq(int* arr, int arrWide, int sqWide);
static int _fillEvensSq(int* arr, int arrWide, int sqWide);
static void _swap(int* a, int* b);
/*********************************************
	function 	: fillTrangl
	Description : 按照顺时针和递增的顺序填充一个正方形螺旋矩阵，返回最后的填充数字
    Parameter   : startnum 矩阵的起始数字，trangeWide 矩阵宽度, array 数组地址(如果是二维数组需要强制转换),arrayWide 数组的行宽度
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
    if( 1 == corner || 2 == corner || 3 == corner )
        loopCount += (tranglWide - 1) * corner;
    else
        corner = 0;
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
/*********************************************
	function 	: magicSquare
	Description : 在给定的数组中填入幻方数字,返回幻方内最大数字+1
    Parameter   : arr是二维数组的一维指针，arrWide是二维数组的第一维大小，sqWide是幻方宽度，arrWide >= sqWide, sqWide > 2
    Author		: zhaoyining
	Date		: 2024-09-11
	History		: 2024-09-11
*********************************************/
int magicSquare(int* arr, int arrWide, int sqWide)
{
    if( NULL == arr || arrWide < sqWide || sqWide < 3 )
        return -1;
    int ret = 0;
    switch( sqWide & 3 ) {
        case 1: default:
            ret = _fillOddSq(arr, arrWide, sqWide, 1);
            break;
        case 2:
            ret = _fillEvenSq(arr, arrWide, sqWide);
            break;
        case 0:
            ret = _fillEvensSq(arr, arrWide, sqWide);
            break;
    }
    return ret;
}
static int _fillOddSq(int* arr, int arrWide, int sqWide, int start)
{
    for( int loc = sqWide - 1 >> 1, i = 0, next = 0; i < sqWide * sqWide; ++i ) {
        arr[loc] = start++;
        next =  loc + (0 == (loc % arrWide + 1) % sqWide ? 1 - sqWide : 1);
        next = next - arrWide >= 0 ? next - arrWide : (next - arrWide + sqWide * arrWide) % (sqWide * arrWide);
        loc = 0 != arr[next] ? (loc + arrWide + sqWide * arrWide) % (sqWide * arrWide) : next;
    }
    return start;
}
static int _fillEvensSq(int* arr, int arrWide, int sqWide)
{
    for( int i = 0, loc = 0; i < sqWide * sqWide; loc += 0 == ++i % sqWide ? 1 + arrWide - sqWide : 1 )
        arr[loc] = i + 1;
    int swapVal = arrWide * (sqWide - 1) + sqWide - 1, childWide = sqWide >> 1;
    if( 4 == sqWide ) {
        _swap(arr, arr + swapVal);
        _swap(arr + arrWide + 1, arr + swapVal - arrWide - 1);
        _swap(arr + arrWide * 2 + 1, arr + swapVal - arrWide * 2 - 1);
        _swap(arr + arrWide * 3, arr + swapVal - arrWide * 3);
    }
    else
        for( int i = 0, locM = 0, locS = childWide - 1; i < sqWide; ++i ) {
            _swap(arr + locM, arr + swapVal - locM);
            _swap(arr + locS, arr + swapVal - locS);
            locM += arrWide + (childWide - 1 != i ? i < childWide ? 1 : -1 : 0);
            locS += arrWide + (childWide - 1 != i ? i < childWide ? -1 : 1 : 0);
        }
    return sqWide * sqWide + 1;
}
static int _fillEvenSq(int* arr, int arrWide, int sqWide)
{
    int childWide = sqWide >> 1, k = sqWide >> 2, start = 1;
    start = _fillOddSq(arr, arrWide, childWide, start);
    start = _fillOddSq(arr + childWide * arrWide + childWide, arrWide, childWide, start);
    start = _fillOddSq(arr + childWide, arrWide, childWide, start);
    start = _fillOddSq(arr + childWide * arrWide, arrWide, childWide, start);
    for( int i = 0; i < childWide; ++i ) {
        for( int j = 0; j < k; ++j )
            _swap(arr + i * arrWide + j + (i == childWide >> 1 ? childWide >> 1 : 0),
                    arr + i * arrWide + childWide * arrWide + j + (i == childWide >> 1 ? childWide >> 1 : 0));
        for( int j = 0; j < k - 1; ++j )
            _swap(arr + i * arrWide + childWide + (childWide >> 1) - j,
                    arr + i * arrWide + childWide * arrWide + childWide + (childWide >> 1) - j);
    }
    return start;
}
static inline void _swap(int* a, int* b)
{
    *a ^= *b ^= *a ^= *b;
}
