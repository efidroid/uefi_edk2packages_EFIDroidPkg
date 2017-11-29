#ifndef __GZIP_DECOMPRESS_H___
#define __GZIP_DECOMPRESS_H___

RETURN_STATUS
EFIAPI
GzipGuidedSectionGetInfo (
  IN  CONST VOID  *InputSection,
  OUT UINT32      *OutputBufferSize,
  OUT UINT32      *ScratchBufferSize,
  OUT UINT16      *SectionAttribute
  );

RETURN_STATUS
EFIAPI
GzipGuidedSectionExtraction (
  IN CONST  VOID    *InputSection,
  OUT       VOID    **OutputBuffer,
  OUT       VOID    *ScratchBuffer,        OPTIONAL
  OUT       UINT32  *AuthenticationStatus
  );

#endif // __GZIP_DECOMPRESS_H__

