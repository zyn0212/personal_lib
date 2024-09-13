/******************************************  
        File name   : zyn_linkedlist.c
        Description : personal linkedlist function library
        Author      : zhaoyining
        Date        : 2021-02-11
        History     : rename file from zynfunc.c
        History     : clean source code
        History     : 2024-09-13 delete addNextXorNode
        History     : 2024-09-13 delete addXorNodeBetweenF_S
    ******************************************/
#include "zyn_func.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define XOR_2PTR(a, b) ((XOR_NODE*)((unsigned long)(a)^(unsigned long)(b)))
#define XOR_3PTR(a, b, c) ((XOR_NODE*)((unsigned long)(a)^(unsigned long)(b)^(unsigned long)(c)))
    /*********************************************
        function    : initXorLinkedList
        Description : 初始化异或链表的头指针，如果参数异常或者内存申请失败，返回NULL，初始化正常返回头指针
        Parameter   : first 链表的第一个节点，second 链表的第二个节点
        Author      : zhaoyining
        Date        : 2022-07-10
        History     : 2022-07-10
    *********************************************/
XOR_HEADER* initXorLinkedList(XOR_NODE* first, XOR_NODE* second)
{
    if( NULL == first || NULL == second )
        return NULL;
    XOR_HEADER* ret = (XOR_HEADER *)calloc(1, sizeof(XOR_HEADER));
    if( NULL == ret )
        return NULL;
    ret->nodeNum = 2;
    ret->first = first;
    ret->first->side = second;
    ret->second = second;
    ret->second->side = first;
    return ret;
}
/*********************************************
    function    : destoryXorLLHeader
    Description : 释放异或链表的头指针，参数为空，返回-1，释放成功返回0
    Parameter   : header 链表的头指针
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
int destoryXorLLHeader(XOR_HEADER* header)
{
    if( NULL == header )
        return -1;
    free(header);
    return 0;
}
/*********************************************
    function    : insertXorNode
    Description : 向异或链表中插入节点，prev 是所插入位置的上一个节点，next是所插入位置的下一个节点，如果插入位置是第一个节点，prev 是NULL，next是first指针地址，如果插入位置是最后一个节点，next是NULL，执行成功返回所插入节点的地址，否则返回NULL
    Parameter   : header 链表的头指针，prev 插入节点的上一个节点，Node当前插入的节点，next 插入节点的下一个节点, confirm 是prev和next节点外侧的节点，用于验证
    Author      : zhaoyining
    Date        : 2024-09-12
    History     : 2024-09-12
*********************************************/
XOR_NODE* insertXorNode(XOR_HEADER* header, XOR_NODE* prev, XOR_NODE* node, XOR_NODE* next, XOR_NODE* confirm)
{
    if( NULL == header || NULL == node || NULL == prev && NULL == next )
        return NULL;
    if( NULL != prev && NULL != next ) {
        if( confirm != XOR_2PTR(prev->side, next) && confirm != XOR_2PTR(next->side, prev) )
            return NULL;
    }
    else if( NULL != prev ) {
       if( confirm != prev->side )
           return NULL;
    }
    else {
       if( confirm != next->side )
           return NULL;
    }
    ++header->nodeNum;
    node->side = XOR_2PTR(prev, next);
    if( NULL != prev )
        prev->side = XOR_3PTR(prev->side, next, node);
    if( NULL != next )
        next->side = XOR_3PTR(next->side, prev, node);
    if( NULL == prev && next == header->first || prev == header->first && NULL == next ) {
        header->second = header->first;
        header->first = node;
    }
    else if( prev == header->first && next == header->second || prev == header->second && next == header->first )
        header->second = node;
    return node;
}
/*********************************************
    function    : deleteXorNode
    Description : 删除异或链表中一个节点，prev是所删节点的上一个节点，next是所删节点的下一个节点，函数只是在链表上删除节点，所删除的节点所在的内存没有释放, 链表节点数量小于等于2或者prev deleteNode next 逻辑上非连续时 ，否则返回NULL, 其他情况返回所删除节点的指针，删除第一个节点时prev是NULL，删除最后一个节点时next是NULL
    Parameter   : header 链表的头指针，prev 当前删除节点的上一个节点，deleteNode 当前删除的节点，next 当前删除节点的下一个节点
    Author      : zhaoyining
    Date        : 2024-09-12
    History     : 2024-09-12
*********************************************/
XOR_NODE* deleteXorNode(XOR_HEADER* header, XOR_NODE* prev, XOR_NODE* node, XOR_NODE* next)
{
    if( NULL == header || NULL == node || header->nodeNum < 3 || NULL == prev && NULL == next )
        return NULL;
    if( node->side != XOR_2PTR(prev, next) )
        return NULL;
    if( NULL != prev )
        prev->side = XOR_3PTR(prev->side, node, next);
    if( NULL != next )
        next->side = XOR_3PTR(next->side, node, prev);
    if( node == header->first ) {
        header->first = header->second;
        header->second = header->first->side;
    }
    else if( node == header->second )
        header->second = XOR_3PTR(prev, next, header->first);
    --header->nodeNum;
    node->side = NULL;
    return node;
}
/*********************************************
    function    : getNextXorNode
    Description : 获取异或链表中current节点的下一个节点，prev是current节点的上一个节点
    Parameter   : current 所获取节点节点的上一个节点，prev current节点的上一个节点
    Author      : zhaoyining
    Date        : 2022-07-10
    History     : 2022-07-10
*********************************************/
XOR_NODE* getNextXorNode(XOR_NODE* prev, XOR_NODE* current)
{  
    if( NULL == current )
        return NULL;
    return XOR_2PTR(current->side, prev);
}
