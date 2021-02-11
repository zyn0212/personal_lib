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
static int check_xornode(HEADER *header, NODE_SMP *node);
static char * huge_pm(char const * a, char const * b, char * result, int maxlen, char inputstatus); /* bit5: a is end, bit4: b is end, bit3: a sgn, bit2: b sgn, bit1 - 0: 10 a max 01 b max */
static char * huge_multi(char * x, char * y, int len, char * result);
extern HEADER * zyn_initxorll(NODE_SMP * head, NODE_SMP * tail)
{
	if( NULL == head || NULL == tail )
		return NULL;
	HEADER header_tmp = {2, head, tail};
	HEADER *header_ret = (HEADER *)calloc(1, sizeof(HEADER));
	if( NULL == header_ret ) {
		return NULL;
	}
	return (HEADER *)memcpy(header_ret, &header_tmp, sizeof(HEADER));
}
extern int zyn_destoryxorll(HEADER * header)
{
	int num = 0;
	if( NULL == header )
		return num;
	NODE_SMP * prev = header->head, * curr = XOR_2PTR(prev->side, header->tail);
	NODE_SMP * next = XOR_2PTR(curr->side, prev);
	while( header->tail != curr ) {
		prev = curr;
		curr = next;
		next = XOR_2PTR(curr->side, prev);
		free(prev);
		++num;
	}
	free(header->head);
	free(header->tail);
	num += 2;
	free(header);
	return -num;
}
extern NODE_SMP * zyn_insertxornode(HEADER * header, NODE_SMP * prev, NODE_SMP * next, NODE_SMP * insnode)
{
	if( NULL == header || NULL == prev || NULL == next || NULL == insnode )
		return NULL;
	int prevloc = check_xornode(header, prev);
	int nextloc = check_xornode(header, next);
	if( -1 != prevloc && -1 != nextloc && (1 == prevloc - nextloc || 1 == nextloc - prevloc) ) {
		prev->side = XOR_3PTR(prev->side, insnode, next);
		insnode->side = XOR_2PTR(prev, next);
		next->side = XOR_3PTR(next->side, prev, insnode);
		++header->nodenum;
		return insnode;
	}
	return NULL;
}
extern NODE_SMP * zyn_deletexornode(HEADER * header, NODE_SMP * prev, NODE_SMP * next, NODE_SMP * delnode)
{
	if( NULL == header || NULL == prev || NULL == next || NULL == delnode )
		return NULL;
	if( header->head == delnode || header->tail == delnode )
		return NULL;
	int prevloc = check_xornode(header, prev);
	int nextloc = check_xornode(header, next);
	int delloc = check_xornode(header, delnode);
	if( -1 != prevloc && -1 != nextloc && -1 != delloc && delloc << 1 == prevloc + nextloc ) {
		prev->side = XOR_3PTR(prev->side, delnode, next);
		delnode->side = XOR_2PTR(prev, next);
		next->side = XOR_3PTR(next->side, prev, delnode);
		--header->nodenum;
		return delnode;
	}
	return NULL;
}
extern int zyn_printxorll(HEADER *header, void (*prtnode)(NODE_SMP *node, int nodeloc), PRTLL_METHOD prtmethod)
{
	if( NULL == header )
		return -1;
	NODE_SMP * prev = NULL, * next = NULL, * current = NULL, * end = NULL;
	if( PRTLL_H2T == prtmethod ) {
		prev = header->tail;
		current = header->head;
		end = header->tail;
	}
	else if( PRTLL_T2H == prtmethod ) {
		prev = header->head;
		current = header->tail;
		end = header->head;
	}
	int loc = 0;
	do {
		if( NULL == prtnode )
			printf("%d\t%d\n", loc, current->val);
		else
			prtnode(current, loc);
		next = XOR_2PTR(current->side, prev);
		prev = current;
		current = next;
		next = NULL;
		++loc;
	} while( end != current );
	if( NULL == prtnode )
		printf("%d\t%d\n", loc, current->val);
	else
		prtnode(current, loc);
	return loc + 1;
}
static int check_xornode(HEADER *header, NODE_SMP *node)
{
	if( header->head == node )
		return 0;
	NODE_SMP * prev = header->tail, * current = header->head, * next = NULL;
	int loc = 0;
	do {
		++loc;
		next = XOR_2PTR(current->side, prev);
		prev = current;
		current = next;
		next = NULL;
		if( current == node )
			return loc;
	} while( header->tail != current );
	return -1;
}
extern char * zyn_huge_minus(char const * a, char const * b, char * result, int maxlen)
{
		return huge_pm(a, b, result, maxlen, 1 << 2);
}
extern char * zyn_huge_plus(char const * a, char const * b, char * result, int maxlen)
{
		return huge_pm(a, b, result, maxlen, 0);
}
static char * huge_pm(char const * a, char const * b, char * result, int maxlen, char inputstatus) /* bit5: a is end, bit4: b is end, bit3: a sgn, bit2: b sgn, bit1 - 0: 10 a max 01 b max */
{
	if( NULL == a || NULL == b || NULL == result || maxlen < 1 )
		return NULL;
	char * const a_stk_top = (char *)calloc(maxlen, sizeof(char));
	char * const b_stk_top = (char *)calloc(maxlen, sizeof(char));
	char * const r_stk_top = (char *)calloc(maxlen, sizeof(char));
	if( NULL == a_stk_top || NULL == b_stk_top || NULL == r_stk_top ) {
		free(a_stk_top);
		free(b_stk_top);
		free(r_stk_top);
		return NULL;
	}
	char * const a_stk_bottom = a_stk_top + maxlen - 1;
	char * const b_stk_bottom = b_stk_top + maxlen - 1;
	char * const r_stk_bottom = r_stk_top + maxlen - 1;
	char * a_stk = a_stk_bottom;
	char * b_stk = b_stk_bottom;
	char * r_stk = r_stk_bottom;
	char a_tmp = 0, b_tmp = 0, upval = 0;
	while( (' ' == *a || '\t' == *a) && '\0' != *a )
		++a;
	if( '-' == *a )
		++a, inputstatus ^= 1 << 3;
	while( '0' == *a && '\0' != *a )
		++a;
	while( (' ' == *b || '\t' == *b) && '\0' != *b )
		++b;
	if( '-' == *b )
		++b, inputstatus ^= 1 << 2;
	while( '0' == *b && '\0' != *b )
		++b;
	while( ('\0' != *a || '\0' != *b) && 3 << 4 != (inputstatus & 3 << 4) ) {
		if( '\0' != (a_tmp = *a) ) {
			++a;
			if( a_stk >= a_stk_top && 0 == (inputstatus & 1 << 5) && a_tmp >= '0' && a_tmp <= '9' )
				*a_stk-- = a_tmp;
			else
				inputstatus |= 1 << 5;
		}
		if( '\0' != (b_tmp = *b) ) {
			++b;
			if( b_stk >= b_stk_top && 0 == (inputstatus & 1 << 4) && b_tmp >= '0' && b_tmp <= '9' )
				*b_stk-- = b_tmp;
			else
				inputstatus |= 1 << 4;
		}
		if( 0 == (inputstatus & 3) ) {
			if( a_tmp > b_tmp )
				inputstatus |= 2;
			else if( b_tmp > a_tmp )
				inputstatus |= 1;
		}
	}
	if( a_stk_bottom != a_stk )
		++a_stk;
	if( b_stk_bottom != b_stk )
		++b_stk;
	if( a_stk_bottom - a_stk != b_stk_bottom - b_stk ) {
		inputstatus &= ~3;
		inputstatus |= a_stk_bottom - a_stk > b_stk_bottom - b_stk ? 2 : 1;
	}
	a_tmp = b_tmp = 0;
	while( a_stk <= a_stk_bottom || b_stk <= b_stk_bottom ) {
		a_tmp = a_stk <= a_stk_bottom ? *a_stk++ - '0' : 0;
		b_tmp = b_stk <= b_stk_bottom ? *b_stk++ - '0' : 0;
		switch( inputstatus & 0xF ) {
			case 0xC: case 0xE: case 0xD:
			case 0x0: case 0x2: case 0x1:
				if( a_tmp + b_tmp + upval > 9 )
					*r_stk-- = a_tmp + b_tmp + upval - 10 + '0', upval = 1;
				else
					*r_stk-- = a_tmp + b_tmp + upval + '0', upval = 0;
				break;
			case 0xA: case 0x6:
				if( a_tmp - upval - b_tmp >= 0 )
					*r_stk-- = a_tmp - upval - b_tmp + '0', upval = 0;
				else
					*r_stk-- = a_tmp - upval - b_tmp + '0' + 10, upval = 1;
				break;
			case 0x9: case 0x5:
				if( b_tmp - upval - a_tmp >= 0 )
					*r_stk-- = b_tmp - upval - a_tmp + '0', upval = 0;
				else
					*r_stk-- = b_tmp - upval - a_tmp + '0' + 10, upval = 1;
				break;
			case 0x8: case 0x4:
				*r_stk-- = '0';
				a_stk = a_stk_bottom + 1;
				b_stk = b_stk_bottom + 1;
			default:
				break;
		}
	}
	if( 0 != upval )
		*r_stk-- = '1';
	if( (inputstatus & 0xC) == 0xC || (inputstatus & 0xF) == 0xA || (inputstatus & 0xF) == 0x5 )
		*r_stk-- = '-';
	++r_stk;
	sprintf(result, "%s", r_stk);
	free(a_stk_top);
	free(b_stk_top);
	free(r_stk_top);
	return result;
}
extern char * zyn_huge_multi(char const * x, char const * y, char * result, int maxlen)
{
	if( NULL == x || NULL == y || NULL == result || maxlen < 1 )
		return NULL;
	char sgn = 0;
	int x_len = 0, y_len = 0;
	while( (' ' == *x || '\t' == *x) && '\0' != *x )
		++x;
	if( '-' == *x )
		++x, sgn ^= 1;
	char const * x_ptr = x;
	while( (' ' == *y || '\t' == *y) && '\0' != *y )
		++y;
	if( '-' == *y )
		++y, sgn ^= 1;
	char const * y_ptr = y;
	while( '\0' != *x )
		if( *x >= '0' && *x <= '9' )
			++x_len, ++x;
		else
			break;
	while( '\0' != *y )
		if( *y >= '0' && *y <= '9' )
			++y_len, ++y;
		else
			break;
	int len = x_len > y_len ? x_len : y_len;
	char * x_tmp = (char *)calloc(len + 1, sizeof(char));
	char * y_tmp = (char *)calloc(len + 1, sizeof(char));
	if( NULL == x_tmp || NULL == y_tmp ) {
		free(x_tmp);
		free(y_tmp);
		return NULL;
	}
	int i = 0;
	for( ; i < len; ++i )
		x_tmp[i] = y_tmp[i] = '0';
	x_tmp[i] = y_tmp[i] = '\0';
	snprintf(x_tmp + len - x_len, x_len + 1, "%s", x_ptr);
	snprintf(y_tmp + len - y_len, y_len + 1, "%s", y_ptr);
	return huge_multi(x_tmp, y_tmp, len, result);
}
static char * huge_multi(char * x, char * y, int len, char * result)
{
	return NULL;
}
