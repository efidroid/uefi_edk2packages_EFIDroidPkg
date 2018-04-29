#ifndef __LIBRARY_STR_H__
#define __LIBRARY_STR_H__

typedef struct {
  CONST CHAR8 *Current;
  CONST CHAR8 *End;
} ASCII_GETSTRING_STATE;

CHAR8 *strtok(CHAR8 *s, CONST CHAR8 *delim);
CHAR8 *strtok_r(CHAR8 *s, CONST CHAR8 *delim, CHAR8 **last);
UINTN strlcat(CHAR8 *dst, CONST CHAR8 *src, UINTN siz);
UINTN strspn(CHAR8 CONST *s, CHAR8 CONST *accept);
CHAR8 *strchr(CONST CHAR8 *s, INTN c);
CHAR8 *strpbrk(CONST CHAR8 *s, CONST CHAR8 *accept);

RETURN_STATUS
EFIAPI
AsciiGetNextString (
  IN OUT ASCII_GETSTRING_STATE   *State,
  OUT    CONST CHAR8             **String
  );

RETURN_STATUS
EFIAPI
AsciiGetString (
  IN     CONST CHAR8             *StringArray,
  IN     UINTN                   MaxSize,
  IN OUT ASCII_GETSTRING_STATE   *State,
  OUT    CONST CHAR8             **String
  );

#endif
