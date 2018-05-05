#include <stdio.h>

int do_dup(int a[], int N)
{
    int sum = 0;
    int sum2 = 0;
    int i;

    for (i = 0; i < N; i++) {
        sum += a[i];
        printf("a[%d]=%d\n", i, a[i]);
    }
    printf("sum=%d\n", sum);

    sum2 = (1 + N - 1)*(N/2);
    printf("sum2=%d\n", sum2);

    return (sum-sum2);
}

void main()
{
    int dup_val = 0;
    int dbg_val = 0;
    int a[] = {1, 2, 3, 4, 4, 5, 6, 7, 8};

    dbg_val = sizeof(a);
    printf("N=%d\n",dbg_val);
    dup_val = do_dup(a, sizeof(a)/sizeof(a[0]));

    printf("%s(%d): dup_val=%d\n", __func__, __LINE__, dup_val);
}
