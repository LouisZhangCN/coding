/*
 * =====================================================================================
 *
 *       Filename:  judge_2.c
 *
 *    Description:  judge the Num is 2^N
 *
 *        Version:  1.0
 *        Created:  05/21/2016 11:01:56 AM
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
#include <stdbool.h>

#define JUDGE_2_N(val) (((val) & (val - 1)) ? false : true)

bool judge_2_n(int val)
{
    int bit_num = sizeof(val) * 8;
    int count = 0;
    int val_tmp = val;

    while (--bit_num) {
        if (val_tmp & 1)
            count++;

        val_tmp >>= 1;
    }

    return (count == 1);
}

void main()
{
    int testnum = 13457;
    //int testnum = 4096;
    bool isnot = false;

    // isnot = judge_2_n(testnum);
    isnot = JUDGE_2_N(testnum);

    if (isnot == true) {
        printf("this num is 2^N\n");
    } else {
        printf("this num is NOT 2^N\n");
    }
}
