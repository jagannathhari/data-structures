#ifndef INCLUDE_SORT
#define INCLUDE_SORT

#define insertion_sort(arr, n, cmp)                                            \
	do {                                                                   \
		int i;                                                         \
		__typeof__(arr[0]) key;                                        \
		for (i = 1; i < n; i++) {                                      \
			int j = i - 1;                                         \
			key = arr[i];                                          \
			while (j >= 0 && cmp(arr[j], key) > 0) {               \
				arr[j + 1] = arr[j];                           \
				j--;                                           \
			}                                                      \
			arr[j + 1] = key;                                      \
		}                                                              \
	} while (0)

#define impl_qsort(t, cmp)                                                     \
	void quick_sort_##t(t *arr, int low, int high)                         \
	{                                                                      \
		if (low < high) {                                              \
			int pivot = arr[high];                                 \
			int temp;                                              \
			int j = low - 1;                                       \
			for (int i = low; i < high; i++) {                     \
				if (cmp(arr[i], pivot) <= 0) {                 \
					j++;                                   \
					temp = arr[j];                         \
					arr[j] = arr[i];                       \
					arr[i] = temp;                         \
				}                                              \
			}                                                      \
			temp = arr[j + 1];                                     \
			arr[j + 1] = arr[high];                                \
			arr[high] = temp;                                      \
			int idx = j + 1;                                       \
			quick_sort_##t(arr, low, idx - 1);                     \
			quick_sort_##t(arr, idx + 1, high);                    \
		}                                                              \
	}

#endif // INCLUDE_SORT
