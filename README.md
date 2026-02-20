# stb-style C Utilities

Single-header, public-domain–style libraries inspired by Sean Barrett’s stb approach. Drop the headers into your project and include them where needed.

## Libraries

### base64.h

Base64 encode/decode helpers.

**API**

```c
char *base64_encode(const unsigned char *src, size_t input_length, char **outptr);
unsigned char *base64_decode(const char *src, size_t input_length, unsigned char **outptr, size_t *outlen);
bool is_valid_base64(const char *base64, size_t input_length);
```

### bitset.h

Simple dynamic bitset macros.

**Macros**

```c
#define BITSET(n) calloc((n+CHAR_BIT-1)/CHAR_BIT,CHAR_BIT)
#define BIT_RESET(bset,pos)  bset[(pos)/CHAR_BIT]&= ~(1<<((pos)%CHAR_BIT))
#define BIT_SET(bset,pos)  bset[(pos)/CHAR_BIT]|= (1<<((pos)%CHAR_BIT))
#define BIT_GET(bset,pos)  bset[(pos)/CHAR_BIT]&(1<<((pos)%CHAR_BIT))
```

### heap.h

Macro-based heap helpers with optional custom comparator.

**Macros**

```c
#define heapify(...)
#define heap_pop(...)
```

## Usage

* Include the header(s) you need.
* No separate build step required.

## Code Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define IMPLEMENT_BASE64
#include "base64.h"
#include "bitset.h"
#include "heap.h"

int main() {
    const char *s = "hello";
    char *enc = 0;
    base64_encode((const unsigned char*)s, strlen(s), &enc);
    if (is_valid_base64(enc, strlen(enc))) {
        unsigned char *dec = 0;
        size_t outlen = 0;
        base64_decode(enc, strlen(enc), &dec, &outlen);
        fwrite(dec, 1, outlen, stdout);
        free(dec);
    }
    free(enc);

    unsigned char *b = BITSET(16);
    BIT_SET(b, 3);
    if (BIT_GET(b, 3)) printf("\nbit 3 set\n");
    BIT_RESET(b, 3);
    free(b);

    int v[] = {5,1,4,2,3};
    heapify(v);
    heap_pop(v);

    return 0;
}
```

## vector.h

Dynamic array macros (generic vectors).

## Code Example

```c
#include <stdio.h>
#include <stdlib.h>

#define IMPLEMENT_VECTOR
#include "vector.h"

int main() {
    int *v = Vector(int);

    vector_append(v, 10);
    vector_append(v, 20);
    vector_append(v, 30);

    for (size_t i = 0; i < vector_length(v); i++) {
        printf("%d ", v[i]);
    }

    int x = vector_pop(v);
    printf("\npopped: %d\n", x);

    free_vector(v);
    return 0;
}
```
## sort.h

Insertion sort macro and quick sort implementation generator (comparator can be a macro).

## Code Example

```c
#include <stdio.h>
#include "sort.h"

#define CMP(a,b) ((a) - (b))

impl_qsort(int, CMP)

int main() {
    int a[] = {5, 2, 4, 1, 3};
    int n = 5;

    insertion_sort(a, n, CMP);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");

    int b[] = {9, 7, 8, 6, 5};
    quick_sort_int(b, 0, 4);
    for (int i = 0; i < 5; i++) printf("%d ", b[i]);
    printf("\n");
    return 0;
}
```
