/*#include <stdio.h>
#include <stdlib.h>


void abort(void) {
    #if defined(__is_libk)
    printf("kernel: panic: abort()\n");
    __asm__ __volatile__("hlt");
    #else
    printf("abort()\n");
    #endif
    while(1) {
        __builtin_unreachable();
    }
}*/