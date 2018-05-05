/*
 * =====================================================================================
 *
 *       Filename:  intvert.c
 *
 *    Description:  invert len from startbit
 *
 *        Version:  1.0
 *        Created:  05/21/2016 11:36:04 AM
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

unsigned int intvert2(unsigned int x, int p, int n)
{
    unsigned int vert_val = 0;

    vert_val = ((2 << n) - 1) << p;

    x ^= vert_val;
    return x;
}

unsigned int intvert(unsigned int x, int p, int n)
{
    int vert_val = 0;
    int get_x_vert_val = 0;
    // int change_vert_val = 0;

    vert_val = (2 << n) - 1;

    get_x_vert_val = (vert_val << p) & x;
    x &= ~(vert_val << p);

    get_x_vert_val >>= p;
    vert_val &= ~(get_x_vert_val);
    x |= vert_val << p;

    return x;
}

void main()
{
    unsigned int x = 0x11;
    int p = 4;
    int n = 3;
    int intvert_val = 0;

    // intvert_val = intvert(x, p, n);
    intvert_val = intvert2(x, p, n);

    printf("x = 0x%x, intvert_val=0x%x\n", x, intvert_val);
}
