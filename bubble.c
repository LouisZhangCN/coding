/*
 * =====================================================================================
 *
 *       Filename:  bubble.c
 *
 *    Description:  bubble.c
 *
 *        Version:  1.0
 *        Created:  06/12/2016 08:04:36 PM
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

#define ARRAY_LEN 7

/* 3, 2, 4, 6, 7, 1, 5 */
/* 3, 2, 4, 6, 7, 1, 5 */

void bubble(int array[], int arr_len)
{
    int i, j;
    int temp = 0;

    for (i = 0; i < arr_len - 1; i++) {
        for (j = 0; j < arr_len - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

int main(void)
{
    int i;
    int array[7] = {3, 2, 4, 6, 7, 1, 5};

    bubble(array, ARRAY_LEN);

    for (i = 0; i < ARRAY_LEN; i++) {
        printf("a[%d]=%d\n", i, array[i]);
    }
}


