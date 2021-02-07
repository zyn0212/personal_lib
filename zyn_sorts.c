/*********************************************
	File name	: zyn_sorts.c
	Description : personal sort function
	Author		: zhaoyining
	Date		: 2021-01-25
	History		: 2021-01-25 creat file with bubble sort
*********************************************/
#include "zynfunc.h"
#include <stdlib.h>
#include <memory.h>
#define ARGUMENT_CHECK(a, b, c, d) do {\
	if( NULL == a || b < 2 || c < 1 )\
		return -1;\
	if( NULL == d )\
		return -2;\
} while(0)
static int swap(void * const a, void * const b, int size);
/*********************************************
	function 	: BubbleSort_z
	Description : bubble sort, return -1 when unsort array have error;
							   return -2 compare function have error
							   return -3 when malloc meet error
							   return 0 or >0 mean array sorted, the value is time of moving element
	Author		: zhaoyining
	Date		: 2021-01-25
	History		: 2021-02-06 optimize code
*********************************************/
int BubbleSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	ARGUMENT_CHECK(unsort, n, size, compare);
	int i = 0, j = 0, ischange = 1, movetimes = 0;
	for( i = 1; i < n && 0 != ischange; ++i )
		for( j = n - 1, ischange = 0; j >= i; --j )
			if( (headIsSmall && compare(unsort + j * size, unsort + (j - 1) * size) < 0)
			 || (!headIsSmall && compare(unsort + j * size, unsort + (j - 1) * size) > 0) )
			{
				if( 0 != swap(unsort + j * size, unsort + (j - 1) * size, size) )
					return -3;
				else
					ischange = ++movetimes > 0;
			}
	return movetimes;
}
/*********************************************
	function 	: SelectSort_z
	Description : Select sort, return -1 when unsort array have error;
							   return -2 compare function have error
							   return -3 when malloc meet error
							   return 0 or >0 mean array sorted, the value is time of moving element
	Author		: zhaoyining
	Date		: 2021-01-25
	History		: 2021-02-07 fix bug and optimize code
*********************************************/
int SelectSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	ARGUMENT_CHECK(unsort, n, size, compare);
	int i = 0, j = 0, movetimes = 0, minloc = 0, maxloc = 0, oddnum = n & 1;
	void *emin = NULL, *emax = NULL;
	for( i = 0; i < n - i; ++i )
	{
		emin = unsort + i * size;
		emax = unsort + i * size;
		for( j = i + 1; j < n - i; ++j )
			if( compare(unsort + j * size, emin) < 0 )
				emin = unsort + j * size;
			else if( compare(unsort + j * size, emax) > 0 )
				emax = unsort + j * size;
		minloc = headIsSmall ? i : n - 1 - i;
		maxloc = headIsSmall ? n - 1 - i : i;
		switch( oddnum )
		{
			case 0://fall through
				if( headIsSmall )
					emin = unsort;
				else
					emax = unsort;
				i = -1;
				oddnum = 1;
				--n;
			case 1:default:
				if( emax == unsort + minloc * size )
					minloc = maxloc;
				if( emin != unsort + minloc * size && ++movetimes > 0 )
					if( 0 != swap(emin, unsort + minloc * size, size) )
						return -3;
				if( emax != unsort + maxloc * size && ++movetimes > 0 )
					if( 0 != swap(emax, unsort + maxloc * size, size) )
						return -3;
				break;
		}
	}
	return movetimes;
}
/*********************************************
	function 	: InsertSort_z
	Description : Insert sort, return -1 when unsort array have error;
							   return -2 compare function have error
							   return -3 when malloc meet error
							   return 0 or >0 mean array sorted, the value is time of moving element
	Author		: zhaoyining
	Date		: 2021-01-26
	History		:
*********************************************/
int InsertSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	ARGUMENT_CHECK(unsort, n, size, compare);
	void *element_tmp = calloc(1, size);
	if( NULL == element_tmp )
		return -3;
	int i = 0, j = 0, movetimes = 0;
	for( i = 1; i < n; ++i )
	{
		for( j = 0; j < i; ++j )
			if( (headIsSmall && compare(unsort + i * size, unsort + j * size) < 0)
			 || (!headIsSmall && compare(unsort + i * size, unsort + j * size) > 0) )
				break;
		if( i != j )
		{
				memcpy(element_tmp, unsort + i * size, size);
				memmove(unsort + (j + 1) * size, unsort + j * size, (i - j) * size);
				memcpy(unsort + j * size, element_tmp, size);
				++movetimes;
		}
	}
	free(element_tmp);
	element_tmp = NULL;
	return movetimes;
}
/*********************************************
	function 	: ShellSort_z
	Description : Shell sort, return -1 when unsort array have error;
							   return -2 compare function have error
							   return -3 when malloc meet error
							   return 0 or >0 mean array sorted, the value is time of moving element
	Author		: zhaoyining
	Date		: 2021-01-26
	History		:
*********************************************/
int ShellSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	if( NULL == unsort || n < 2 || size < 1 )
		return -1;
	if( NULL == compare )
		return -2;
	void *element_tmp = calloc(1, size);
	if( NULL == element_tmp )
		return -3;
	int gap = 1, i = 0, j = 0, k = 0, dest = 0, movetimes = 0;
	while( gap < n / 3 )
		gap = gap * 3 + 1;
	for( ; gap > 0; gap = (gap - 1) / 3 )
		for( i = gap; i < n; ++i )
		{
			for( j = i, dest = i; j > gap - 1; j -= gap )
				if( (headIsSmall && compare(unsort + i * size, unsort + (j - gap) * size) < 0)
				 || (!headIsSmall && compare(unsort + i * size, unsort + (j - gap) * size) > 0) )
					dest = j - gap;
			if( dest != i )
			{
				memcpy(element_tmp, unsort + i * size, size);
				for( k = i; k > dest; k -= gap )
					memcpy(unsort + k * size, unsort + (k - gap) * size, size);
				memcpy(unsort + dest * size, element_tmp, size);
				++movetimes;
			}
		}
	free(element_tmp);
	element_tmp = NULL;
	return movetimes;
}
/*********************************************
	function 	: MergeSort_z
	Description : Merge sort, return -1 when unsort array have error;
							   return -2 compare function have error
							   return -3 or < -3 when malloc meet error
							   return 0 mean array sorted, the value is time of moving element
	Author		: zhaoyining
	Date		: 2021-01-28
	History		:
*********************************************/
int MergeSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	if( NULL == unsort || n < 2 || size < 1 )
		return -1;
	if( NULL == compare )
		return -2;
	int ret = 0;
	switch( n )
	{
		case 2:
			if( (headIsSmall && compare(unsort + size, unsort) < 0)
			 || (!headIsSmall && compare(unsort + size, unsort) > 0) )
				 swap(unsort, unsort + size, size);
			ret = 0;
			break;
		default:
			ret = MergeSort_z(unsort, n >> 1, size, compare, headIsSmall) + MergeSort_z(unsort + (n >> 1) * size, n - (n >> 1), size, compare, headIsSmall);
			break;
	}
	void *element_tmp = calloc(n, size);
	if( NULL == element_tmp )
		ret += -3;
	void *element_left = unsort, *element_right = unsort + (n >> 1) * size, *element_head = element_tmp;
	int i = 0;
	for( ; i < n; ++i )
		if( element_left >= unsort + (n >> 1) * size )
		{
			memcpy(element_tmp, element_right, size);
			element_tmp += size;
			element_right += size;
		}
		else if( element_right >= unsort + (n - 1) * size )
		{
			memcpy(element_tmp, element_left, size);
			element_tmp += size;
			element_left += size;
		}
		else
		{
			if( (headIsSmall && compare(element_left, element_right) < 0)
			 || (!headIsSmall && compare(element_left, element_right) > 0) )
			{
				memcpy(element_tmp, element_left, size);
				element_tmp += size;
				element_left += size;
			}
			else
			{
				memcpy(element_tmp, element_right, size);
				element_tmp += size;
				element_right += size;
			}
		}
	free(element_tmp);
	element_tmp = NULL;
	return ret;
}
/*********************************************
	function 	: QuickSort_z
	Description : Quick sort, return -1 when unsort array have error;
							  return -2 compare function have error
							  return 0 mean array sorted, the value is time of moving element
	Author		: zhaoyining
	Date		: 2021-01-28
	History		:
*********************************************/
int QuickSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	if( NULL == unsort || n < 2 || size < 1 )
		return -1;
	if( NULL == compare )
		return -2;
	void *element_gap = unsort, *element_left = unsort + size, *element_right = unsort + (n - 1) * size;
	int find_left = 0, find_right = 0, left_num = 0;
	while( element_right > element_left )
	{
		for( ; element_left < element_right; element_right -= size )
			if( (headIsSmall && compare(element_right, element_gap) < 0)
			|| (!headIsSmall && compare(element_right, element_gap) > 0) )
				break;
		for( left_num = 1; element_left < element_right; element_left += size, ++left_num )
			if( (headIsSmall && compare(element_right, element_gap) < 0)
			|| (!headIsSmall && compare(element_right, element_gap) > 0) )
				break;
		if( element_left < element_right )
			swap(element_left, element_right, size);
	}
	swap(element_right, element_gap, size);
	QuickSort_z(unsort, left_num, size, compare, headIsSmall);
	QuickSort_z(unsort + left_num * size, n - left_num, size, compare, headIsSmall);
	return 0;
}
/*********************************************
	function 	: CountSort_z
	Description : Count sort, return -1 when unsort array have error;
							  return -2 compare function have error
							  return -3 or < -3 when malloc meet error
							  return 0 mean array sorted, the value is time of moving element
							  same key of elements is 10
	Author		: zhaoyining
	Date		: 2021-01-28
	History		:
*********************************************/
int CountSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	typedef struct {
		int count;
		int location[10];
	} CC;
	if( NULL == unsort || n < 2 || size < 1 )
		return -1;
	if( NULL == compare )
		return -2;
	int i = 0, j = 0, length = 0, curnum = 0;
	void *element_small = unsort, *element_large = unsort;
	for( ; i < n; ++i )
		if( compare(unsort + i * size, element_small) < 0 )
			element_small = unsort + i * size;
		else if( compare(unsort + i * size, element_large) > 0 )
			element_large = unsort + i * size;
	length = compare(element_large, element_small) + 1;
	CC *result = (CC *)calloc(length, sizeof(CC));
	if( NULL == result )
		return -3;
	void *sorted = calloc(n, size);
	if( NULL == result )
	{
		free(result);
		result = NULL;
		return -4;
	}
	for( i = 0; i < n; ++i )
	{
		curnum = compare(unsort + i * size, element_small);
		if( result[curnum].count < 9 )
			result[curnum].location[result[curnum].count] = i;
		++result[curnum].count;
	}
	for( i = 0, curnum = 0; i < length; ++i )
		for( j = 0; j < result[i].count; ++j, ++curnum )
			if( j < 10 )
				memcpy(sorted + curnum * size, unsort + result[i].location[j] * size, size);
			else
				memcpy(sorted + curnum * size, unsort + result[i].location[9] * size, size);
	memcpy(unsort, sorted, n * size);
	if( curnum != n )
		return -5;
	free(result);
	free(sorted);
	result = NULL;
	sorted = NULL;
	return 0;
}
/*********************************************
	function 	: RadixSort_z
	Description : Radix sort, return -1 when unsort array have error;
							  return -2 compare function have error
							  return -3 or < -3 when malloc meet error
							  return 0 mean array sorted, the value is time of moving element
							  same key of elements is 10
	Author		: zhaoyining
	Date		: 2021-01-29
	History		:
*********************************************/
int RadixSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	typedef struct {
		int value;
		int location;
	} CC;
	if( NULL == unsort || n < 2 || size < 1 )
		return -1;
	if( NULL == compare )
		return -2;
	CC *unsort_tmp = calloc(n, sizeof(CC));
	if( NULL == unsort_tmp )
		return -3;
	void *element_small = unsort, *element_large = unsort;
	int radix = 0, i = 0, j = 0, curnum = 0, count = 0, k = 0;
	for( ; i < n; ++i )
		if( compare(unsort + i * size, element_small) < 0 )
			element_small = unsort + i * size;
		else if( compare(unsort + i * size, element_large) > 0 )
			element_large = unsort + i * size;
	int length = compare(element_large, element_small);
	for( i = 0; i < n; ++i )
	{
		unsort_tmp[i].value = compare(unsort + i * size, element_small);
		unsort_tmp[i].location = i;
	}
	while( 0 != length )
	{
		++radix;
		length >>= 4;
	}
	CC *radixarray = calloc((n + 1) << 4, sizeof(CC));
	if( NULL == radixarray )
	{
		free(unsort_tmp);
		unsort_tmp = NULL;
		return -4;
	}
	for( i = 0; i < radix; ++i )
	{
		for( j = 0; j < n; ++j )
		{
			curnum = unsort_tmp[j].value >> (i << 2) & 0xF;
			count = ++radixarray[curnum * (n + 1)].value;
			memcpy(radixarray + curnum * (n + 1) + count, unsort_tmp + j, sizeof(CC));
		}
		for( j = 0, curnum = 0; j < 16; ++j )
			for( k = 0; k < radixarray[j * (n + 1)].value; ++k, ++curnum )
				memcpy(unsort_tmp + curnum, radixarray + j * (n + 1) + 1 + k, sizeof(CC));
		memset(radixarray, 0, sizeof(CC) * ((n + 1) << 4));
	}
	void *sorted = calloc(n, size);
	if( NULL == sorted )
	{
		free(unsort_tmp);
		free(radixarray);
		unsort_tmp = NULL;
		radixarray = NULL;
		return -5;
	}
	for( i = 0; i < n; ++i )
		if( headIsSmall )
			memcpy(sorted + i * size, unsort + unsort_tmp[i].location * size, size);
		else
			memcpy(sorted + i * size, unsort + unsort_tmp[n - 1 - i].location * size, size);
	memcpy(unsort, sorted, n * size);
	free(unsort_tmp);
	free(sorted);
	free(radixarray);
	unsort_tmp = NULL;
	sorted = NULL;
	radixarray = NULL;
	return 0;
}
/*********************************************
	function 	: BucketSort_z
	Description : Bucket sort, return -1 when unsort array have error;
							  return -2 compare function have error
							  return -3 or < -3 when malloc meet error
							  return 0 mean array sorted, the value is time of moving element
							  same key of elements is 10
	Author		: zhaoyining
	Date		: 2021-01-31
	History		:
*********************************************/
int BucketSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	typedef struct bucket {
		int value;
		int location;
		struct bucket *next;
	} CC;
	if( NULL == unsort || n < 2 || size < 1 )
		return -1;
	if( NULL == compare )
		return -2;
	void *element_small = unsort, *element_large = unsort;
	CC *unsort_tmp = calloc(n, sizeof(CC));
	if( NULL == unsort_tmp )
		return -3;
	int i = 0, length = 0, bucketnum = 0, curnum = 0, j = 0;
	CC *curbucket = NULL, *curbucketnext_tmp = NULL;
	for( ; i < n; ++i )
		if( compare(unsort + i * size, element_small) < 0 )
			element_small = unsort + i * size;
		else if( compare(unsort + i * size, element_large) > 0 )
			element_large = unsort + i * size;
	length = compare(element_large, element_small);
	for( i =0; i < n; ++i )
	{
		unsort_tmp[i].value = compare(unsort + i * size, element_small);
		unsort_tmp[i].location = i;
		unsort_tmp[i].next = NULL;
	}
	bucketnum = (length + 15) >> 4;
	if( 0 == bucketnum )
		++bucketnum;
	CC *buckethead = calloc(bucketnum, sizeof(CC));
	if( NULL == buckethead )
	{
		free(unsort_tmp);
		unsort_tmp = NULL;
		return -4;
	}
	void * sorted = calloc(n, size);
	if( NULL == sorted )
	{
		free(unsort_tmp);
		unsort_tmp = NULL;
		free(buckethead);
		buckethead = NULL;
		return -5;
	}
	for( i = 0; i < n; ++i )
	{
		curnum = unsort_tmp[i].value >> 4;
		curbucket = buckethead + curnum;
		while( NULL != curbucket->next )
		{	
			if( (headIsSmall && unsort_tmp[i].value <= curbucket->next->value)
			|| (!headIsSmall && unsort_tmp[i].value >= curbucket->next->value) )
				break;
			curbucket = curbucket->next;
		}
		curbucketnext_tmp = curbucket->next;
		curbucket->next = unsort_tmp + i;
		curbucket->next->next = curbucketnext_tmp;
	}
	for( i = 0, curnum = 0; i < bucketnum; ++i )
	{
		j = headIsSmall ? i : bucketnum - 1 - i;
		for( curbucket = buckethead[j].next; NULL != curbucket; ++curnum, curbucket = curbucket->next )
			memcpy(sorted + curnum * size, unsort + curbucket->location * size, size);
	}
	memcpy(unsort, sorted, n * size);
	free(unsort_tmp);
	unsort_tmp = NULL;
	free(buckethead);
	buckethead = NULL;
	free(sorted);
	sorted = NULL;
	return 0;
}
typedef struct bucket {
	int value;
	int location;
	struct bucket *left;
	struct bucket *right;
} CC;
static void fixheap(CC *parent, int headIsSmall);
/*********************************************
	function 	: HeapSort_z
	Description : Heap sort, return -1 when unsort array have error;
							  return -2 compare function have error
							  return -3 or < -3 when malloc meet error
							  return 0 mean array sorted, the value is time of moving element
							  same key of elements is 10
	Author		: zhaoyining
	Date		: 2021-02-02
	History		:
*********************************************/
int HeapSort_z(void *unsort, int n, int size, int (*compare)(void const *a, void const *b), int headIsSmall)
{
	if( NULL == unsort || n < 2 || size < 1 )
		return -1;
	if( NULL == compare )
		return -2;
	CC *unsort_tmp = calloc(n + 1, sizeof(CC));
	if( NULL == unsort_tmp )
		return -3;
	int i = 0;
	void *element_small = unsort, *element_large = unsort;
	CC *parent = NULL, *left = NULL, *right = NULL, *pp = NULL;
	for( ; i < n; ++i )
		if( compare(unsort + i * size, element_small) < 0 )
			element_small = unsort + i * size;
		else if( compare(unsort + i * size, element_large) > 0 )
			element_large = unsort + i * size;
	unsort_tmp[n].value = n;
	unsort_tmp[n].location = -1;
	unsort_tmp[n].left = unsort_tmp;
	unsort_tmp[0].location = 0;
	unsort_tmp[0].value = compare(unsort, element_small);
	for( i = 1; i < n; ++i )
	{
		unsort_tmp[i].value = compare(unsort + i * size, element_small);
		unsort_tmp[i].location = i;
		if( 1 == (i & 1) )
			unsort_tmp[(i - 1) >> 1].left = unsort_tmp + i;

		else
			unsort_tmp[(i - 1) >> 1].right = unsort_tmp + i;
	}
	for( i = n - 1; i > 0; --i )
	{
		fixheap(unsort_tmp + ((i - 1) >> 1), headIsSmall);
	}
	for( i = 0; i < n - 1; ++i )
	{
		left = unsort_tmp->left;
		right = unsort_tmp->right;
		swap(unsort_tmp, unsort_tmp + n - 1 - i, sizeof(CC));
		unsort_tmp->left = left;
		unsort_tmp->right = right;
		if( (n - 1 - i & 1) == 1 )
			unsort_tmp[(n - 1 - i - 1) >> 1].left = NULL;
		else
			unsort_tmp[(n - 1 - i - 1) >> 1].right = NULL;
		fixheap(unsort_tmp, headIsSmall);
	}
	void *sorted = calloc(n, size);
	for( i = 0; i < n; ++i )
		memcpy(sorted + i * size, unsort + unsort_tmp[i].location * size, size);
	memcpy(unsort, sorted, n * size);
	return 0;
}
int swap(void * const a, void * const b, int size)
{
	void *tmp = calloc(1, size);
	if( NULL == tmp )
		return -1;
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
	free(tmp);
	tmp = NULL;
	return 0;
}
void fixheap(CC *parent, int headIsSmall)
{
	CC *parentleft = NULL, *parentright = NULL, *childleft = NULL, *childright = NULL;
	if( NULL != parent->right && ((headIsSmall && parent->right->value < parent->value) || (!headIsSmall && parent->right->value > parent->value)) )
	{
		parentleft = parent->left;
		parentright = parent->right;
		childleft = parent->right->left;
		childright = parent->right->right;
		swap(parent, parent->right, sizeof(CC));
		parent->left = parentleft;
		parent->right = parentright;
		parent->right->left = childleft;
		parent->right->right = childright;
		fixheap(parent->right, headIsSmall);
	}
	if( NULL != parent->left && ((headIsSmall && parent->left->value < parent->value) || (!headIsSmall && parent->left->value > parent->value)) )
	{
		parentleft = parent->left;
		parentright = parent->right;
		childleft = parent->left->left;
		childright = parent->left->right;
		swap(parent, parent->left, sizeof(CC));
		parent->left = parentleft;
		parent->right = parentright;
		parent->left->left = childleft;
		parent->left->right = childright;
		fixheap(parent->left, headIsSmall);
	}
}
