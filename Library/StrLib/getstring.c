#include <Library/BaseLib.h>
#include <Library/StrLib.h>

RETURN_STATUS
EFIAPI
AsciiGetNextString (
  IN OUT ASCII_GETSTRING_STATE   *State,
  OUT    CONST CHAR8             **String
  )
{
  if (State->Current >= State->End) {
    return RETURN_NOT_FOUND;
  }

  UINTN CurrentLength = AsciiStrnSizeS(State->Current, State->End - State->Current);
  if (State->Current + CurrentLength > State->End) {
    return RETURN_UNSUPPORTED;
  }

  *String = State->Current;
  State->Current += CurrentLength;

  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
AsciiGetString (
  IN     CONST CHAR8             *StringArray,
  IN     UINTN                   MaxSize,
  IN OUT ASCII_GETSTRING_STATE   *State,
  OUT    CONST CHAR8             **String
  )
{
  State->Current = StringArray;
  State->End = StringArray + MaxSize;

  return AsciiGetNextString(State, String);
}
