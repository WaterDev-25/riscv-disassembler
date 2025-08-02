#ifndef SECTION_FINDER_H_
    #define SECTION_FINDER_H_

    #include "binary_reader.h"
    #include "def.h"
    #include <stdint.h>

typedef struct {
    BinaryReaderPtr _binary;

    uint64_t        _textOffset;
    uint64_t        _textSize;
    uint64_t        _entryPointAddr;
} SectionFinder, *SectionFinderPtr;

SectionFinderPtr InitSectionFinder( BinaryReaderPtr binary );
STATUS DestroySectionFinder( SectionFinderPtr section );

#endif /* !SECTION_FINDER_H_ */
