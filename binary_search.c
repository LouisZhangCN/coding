/*
 * =====================================================================================
 *
 *       Filename:  binary_search.c
 *
 *    Description:  binary_search algorithm implementation
 *
 *        Version:  1.0
 *        Created:  05/24/2016 09:20:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), lolkokpop@126.com
 *   Organization:  No Orgnization
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

unsigned int binary_search(int test_array[], int search_val, int low, int high)
{
    int mid, mid_offset;

    mid = (low + high)/ 2;
    if (mid > 0) {
        mid_offset = (low + high) % 2;
        printf("mid_offset=%d\n", mid_offset);
    }
    else {
        // mid_offset = 0;
        return mid;
    }

    printf("mid=%d, mid_offset=%d, element=%d\n", mid, mid_offset, test_array[mid - mid_offset]);
    if (search_val > test_array[mid - mid_offset]) {
        // printf("search_val=%d, test_array[%d]=%d\n", search_val, mid-mid_offset, test_array[mid-mid_offset]);
        return binary_search(test_array + mid, search_val, mid, high);
    }
    else if (search_val < test_array[mid - mid_offset]) {
        // printf("search_val=%d, test_array[%d]=%d\n", search_val, mid-mid_offset, test_array[mid-mid_offset]);
        return binary_search(test_array, search_val, low, mid - mid_offset);
    }
    else {
        return mid;
    }
}


int main(void)
{
    int test_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int test_array_copy[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    int search_val = 1;
    int search_val_copy = 10;
    unsigned int find_idx = 0;
    unsigned int find_idx_copy = 0;

    find_idx = binary_search(test_array, search_val, 0, sizeof(test_array)/sizeof(test_array[0]) - 1);
    printf("find_idx=%d\n", find_idx);
    find_idx_copy = binary_search(test_array_copy, search_val_copy, 0, sizeof(test_array_copy)/sizeof(test_array_copy[0]) - 1);
    printf("find_idx_copy=%d\n", find_idx_copy);
}
