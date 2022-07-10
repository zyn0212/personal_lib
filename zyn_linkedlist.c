/******************************************  
	File name	: zyn_linkedlist.c
	Description : personal linkedlist function library
	Author		: zhaoyining
	Data		: 2021-02-11
	History		: rename file from zynfunc.c
******************************************/
#include "./zynfunc.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define XOR_2PTR(a, b) ((NODE_SMP*)((long)(a)^(long)(b)))
#define XOR_3PTR(a, b, c) ((NODE_SMP*)((long)(a)^(long)(b)^(long)(c)))
HEADER* initXorLinkedList(NODE_SMP* first, NODE_SMP* second, int nodeNum)
{
	if( NULL == first || NULL == second || nodeNum < 2 )
		return NULL;
	HEADER *header_ret = (HEADER *)calloc(1, sizeof(HEADER));
	if( NULL == header_ret ) {
		return NULL;
	}
	HEADER header_tmp = {nodeNum, first, second};
	return (HEADER *)memcpy(header_ret, &header_tmp, sizeof(HEADER));
}
int destoryXorLinkedList(HEADER * header)
{
	if( NULL == header )
		return -1;
	free(header);
	return 0;
}
NODE_SMP* addNextXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* current, NODE_SMP* newNode)
{
	if( NULL == header || NULL == prev || NULL == current || NULL == newNode )
		return NULL;
    current->side = XOR_2PTR(prev, newNode);
    ++header->nodeNum;
    return newNode;
}
NODE_SMP* deleteXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* deleteNode, NODE_SMP* next)
{
	if( NULL == header || NULL == prev || NULL == deleteNode || NULL == next || header->nodeNum < 3 )
		return NULL;
    prev->side = XOR_3PTR(prev->side, deleteNode, next);
    next->side = XOR_3PTR(next->side, deleteNode, prev);
    --header->nodeNum;
    return deleteNode;
}
NODE_SMP* getNextXorNode(NODE_SMP* prev, NODE_SMP* current)
{
    if( NULL == prev || NULL == current )
        return NULL;
    return XOR_2PTR(current->side, prev);
}
