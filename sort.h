#ifndef INCLUDE_SORT
#define INCLUDE_SORT

#define sort_cmp(a, b) ((a) > (b))

#define insertion_sort(arr, n, cmp)                                            \
    do                                                                         \
    {                                                                          \
        int i;                                                                 \
        __typeof__(arr[0]) key;                                                \
        for (i = 1; i < n; i++)                                                \
        {                                                                      \
            int j = i - 1;                                                     \
            key = arr[i];                                                      \
            while (j > 0 && cmp(arr[j], key))                                  \
            {                                                                  \
                arr[j + 1] = arr[j];                                           \
                j--;                                                           \
            }                                                                  \
            arr[j + 1] = key;                                                  \
        }                                                                      \
    } while (0)

#endif // INCLUDE_SORT
