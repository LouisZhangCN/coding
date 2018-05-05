/*
 * =====================================================================================
 *
 *       Filename:  n_factorial.c
 *
 *    Description:  compute n factorial
 *
 *        Version:  1.0
 *        Created:  05/22/2016 09:00:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), louis.zhang@126.com
 *   Organization:  No Orgnization
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

int fact(int num)
{
    if ((num == 0) || (num == 1)) {
        return 1;
    } else {
        return num * fact(num -1);
    }
}

void main()
{
    int val = 4;
    int final_val = 0;

    final_val = fact(val);

    printf("final_val=%d\n", final_val);
}
