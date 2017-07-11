#ifndef _STRING_H
#define _STRING_H

#include <stdint.h>
#include <Library/StrLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

#define strncat(dest, src, n) AsciiStrnCatS((dest), (n), (src), (n))
#define strncmp(s1, s2, n) ((int)AsciiStrnCmp((s1), (s2), (n)))
#define strlen(s) ((size_t)AsciiStrLen((s)))
#define strncpy(dest, src, n) AsciiStrCpyS((dest), (n), (src))
#define strstr(s1 , s2) AsciiStrStr((s1), (s2))
#define memset(s, c, n) SetMem((s), (UINTN)(n), (UINT8)(c))
#define memcpy(s1, s2, n) CopyMem((s1), (s2), (n))

//
// UNSAFE
//
#define strcpy(dest, src) AsciiStrCpyS((dest), AsciiStrLen((src)), (src))

#endif
