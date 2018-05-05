/*
 * =====================================================================================
 *
 *       Filename:  qsort.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/12/2016 08:39:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#define ARRAY_LEN 8

int partition(int array[], int low, int high)
{
    int temp = array[low];
    int sign = array[low];

    while (low < high)
    {
        while (low < high && array[high] >= sign)
            high--;
        array[low] = array[high];
        while (low < high && array[low] <= sign)
            low++;
        array[high] = array[low];
    }
    array[low] = temp;

    return low;
}

void __qsort(int array[], int low, int high)
{
    int mid;

    if (low < high) {
        mid = partition(array, low, high);
        __qsort(array, low, mid - 1);
        __qsort(array, mid + 1, high);
    }
}

int main()
{
    int array[8] = {8, 1, 7,  3, 2, 4, 6, 5};
    int low = 0, high = ARRAY_LEN - 1;
    int i;

    __qsort(array, low, high);

    for (i = 0; i < ARRAY_LEN; i++) {
        printf("a[%d]=%d\n", i, array[i]);
    }


}

