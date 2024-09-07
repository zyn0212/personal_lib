/******************************************  
    File name   : zyn_linkedlist.c
    Description : personal linkedlist function library
    Author      : zhaoyining
    Data        : 2021-02-11
    History     : rename file from zynfunc.c
    History     : clean source code
******************************************/
#include "zyn_func.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define XOR_2PTR(a, b) ((NODE_SMP*)((long)(a)^(long)(b)))
#define XOR_3PTR(a, b, c) ((NODE_SMP*)((long)(a)^(long)(b)^(long)(c)))
/*********************************************
    function    : initXorLinkedList
    Description : 初始化异或链表的头指针，如果参数异常或者内存申请失败，返回NULL，初始化正常返回头指针
    Parameter   : first 链表的第一个节点，second 链表的第二个节点，nodeNum 初始化时链表节点数量
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
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
/*********************************************
    function    : destoryXorLLHeader
    Description : 释放异或链表的头指针，参数为空，返回0，否则返回1，注意:释放头指针后可能无法获取链表节点
    Parameter   : header 链表的头指针
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
int destoryXorLLHeader(HEADER * header)
{
    if( NULL == header )
        return -1;
    free(header);
    return 0;
}
/*********************************************
    function    : addNextXorNode
    Description : 向异或链表中插入节点，具体位置 prev节点是所增加节点的上一个节点，next节点是所增加节点的下一个节点, 函数无法向first和second之间增加节点
    Parameter   : header 链表的头指针，prev 插入节点的上一个节点，newNode当前插入的节点，next 插入节点的下一个节点
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
NODE_SMP* insertNewXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* newNode, NODE_SMP* next)
{
    if( NULL == header || NULL == prev || NULL == newNode || NULL == next )
        return NULL;
    prev->side = XOR_3PTR(prev->side, next, newNode);
    newNode->side = XOR_2PTR(prev, next);
    next->side = XOR_3PTR(next->side, prev, newNode);
    ++header->nodeNum;
    return newNode;
}
/*********************************************
    function    : addNextXorNode
    Description : 向异或链表中增加节点，具体位置 current节点是所增加节点的上一个节点，prev节点是current节点的下一个节点, 如果current的下一个节点非空，函数返回NULL
    Parameter   : header 链表的头指针，current 插入节点的下一个节点，prev current节点的上一个节点，newNode当前插入的节点
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
extern NODE_SMP* addNewXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* current, NODE_SMP* newNode)
{
    if( NULL == header || NULL == prev || NULL == current || NULL == newNode )
        return NULL;
    if( NULL != XOR_2PTR(current->side, prev) )
        return NULL;
    current->side = XOR_2PTR(current->side, newNode);
    newNode->side = XOR_2PTR(newNode->side, current);
    ++header->nodeNum;
    return newNode;
}
/*********************************************
    function    : addXorNodeBetweenF_S
    Description : 向异或链表中first和second之间增加节点
    Parameter   : header 链表的头指针，prev 当前插入节点的上一个节点
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
NODE_SMP* addXorNodeBetweenF_S(HEADER* header, NODE_SMP* prev, NODE_SMP* newNode)
{
    NODE_SMP* nodeMoved = NULL;
    if( header->first == prev ) {
        nodeMoved = header->second;
        HEADER tmpHeader = {header->nodeNum, header->first, newNode};
        memcpy(header, &tmpHeader, sizeof tmpHeader);
    }
    else if( header->second == prev ) {
        nodeMoved = header->first;
        HEADER tmpHeader = {header->nodeNum, newNode, header->second};
        memcpy(header, &tmpHeader, sizeof tmpHeader);
    }
    addNewXorNode(header, prev, newNode, nodeMoved);
}
/*********************************************
    function    : deleteXorNode
    Description : 删除异或链表中一个节点，prev是所删节点的上一个节点，next时所删节点的下一个节点, next节点补充所删除节点的逻辑位置，函数只是在链表上删除节点，所删除的节点所在的内存没有释放
                  链表节点数量小于等于2或者prev deleteNode next 逻辑上非连续时 ，否则返回NULL, 其他情况返回所删除节点指针，prev和next可以有一个为NULL
    Parameter   : header 链表的头指针，prev 当前删除节点的上一个节点，deleteNode 当前删除的节点，next 当前删除节点的下一个节点
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
NODE_SMP* deleteXorNode(HEADER* header, NODE_SMP* prev, NODE_SMP* deleteNode, NODE_SMP* next)
{
    if( NULL == header || NULL == deleteNode || header->nodeNum < 3 )
        return NULL;
    if( NULL == prev && NULL == next )
        return NULL;
    if( deleteNode->side != XOR_2PTR(prev, next) )
        return NULL;
    if( header->first == prev && header->second == deleteNode ) {
        HEADER tmpHeader = {header->nodeNum, prev, next};
        memcpy(header, &tmpHeader, sizeof tmpHeader);
    }
    else if( header->first == next && header->second == deleteNode ) {
        if( NULL == XOR_2PTR(next->side, deleteNode) )
            return NULL;
        HEADER tmpHeader = {header->nodeNum, XOR_2PTR(next->side, deleteNode), next};
        memcpy(header, &tmpHeader, sizeof tmpHeader);
    }
    else if( header->first == deleteNode && header->second == next ) {
        if( NULL == XOR_2PTR(next->side, deleteNode) )
            return NULL;
        HEADER tmpHeader = {header->nodeNum, next, XOR_2PTR(next->side, deleteNode)};
        memcpy(header, &tmpHeader, sizeof tmpHeader);
    }
    else if( header->first == deleteNode && header->second == prev ) {
        HEADER tmpHeader = {header->nodeNum, next, prev};
        memcpy(header, &tmpHeader, sizeof tmpHeader);
    }
    if( NULL != prev )
        prev->side = XOR_3PTR(prev->side, deleteNode, next);
    if( NULL != next )
        next->side = XOR_3PTR(next->side, deleteNode, prev);
    --header->nodeNum;
    return deleteNode;
}
/*********************************************
    function    : destoryXorLinkedList
    Description : 获取异或链表中current节点的下一个节点，prev是current节点的上一个节点
    Parameter   : current 所获取节点节点的上一个节点，prev current节点的上一个节点
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
NODE_SMP* getNextXorNode(NODE_SMP* prev, NODE_SMP* current)
{  
    if( NULL == prev || NULL == current )
        return NULL;
    return XOR_2PTR(current->side, prev);
}
