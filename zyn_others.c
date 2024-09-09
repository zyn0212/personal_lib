/*********************************************
	File name	: zyn_interest.c
	Description : personal insterest function
	Author		: zhaoyining
	Date		: 2022-07-05
	History		: 2022-07-05 creat file with fillTrangl function
	              2024-07-04 add zyn_cvtDecToX function
    Histroy     : delete  convertHex2Oct function
    Histroy     : delete  zyn_cvtDecToX function
*********************************************/
#include "zyn_func.h"
#include <stdio.h>
#define MAX_CONVERT_LENGTH 500
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
