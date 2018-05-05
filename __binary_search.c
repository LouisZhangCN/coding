/*
 * =====================================================================================
 *
 *       Filename:  __binary_search.c
 *
 *    Description:  i
 *
 *        Version:  1.0
 *        Created:  06/12/2016 09:47:43 PM
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

#define ARRAY_LEN 10

int __binary_search(int array[], int size, int key)
{
    int low = 0, high = size - 1, mid;

    while (low <= high) {
        mid = (low + high) / 2;

        if (array[mid] == key)
            return mid;
        if (array[mid] > key)
            high = mid - 1;
        if (array[mid] < key)
            low = mid + 1;
    }

    return -1;
}

int main()
{
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int key = 3;
    int index = 0;

    index = __binary_search(array, ARRAY_LEN, key);

    printf("id=%d\n", index);
}
