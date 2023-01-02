#ifndef STRING_H
#define STRING_H 
#include "types.h"
#include <stddef.h>
#include "cdefs.h"


int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);

uint16 strlength(string ch) {
	uint16 i = 1;
	while(ch[i++]); //Go through the string until false
	return --i;
}

uint8 strEql(string ch1, string ch2){
	uint8 result = 1;
	uint8 size = strlength(ch1);

	if (size != strlength(ch2)) result = 0;
	else {
		uint8 i = 0;
		for (i;i<=size;i++) {					// Compare characters individualy
			if (ch1[i] != ch2[i]) result = 0;
		}
	}
	return result;
}


#endif