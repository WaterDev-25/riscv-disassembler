#ifndef BINARY_READER_H_
    #define BINARY_READER_H_

    #include "def.h"

typedef struct {
    char *_fileName;
    char *_buffer;
} BinaryReader, *BinaryReaderPtr;

BinaryReader *InitBinaryReader(char *fileName);
FNSTATUS DestroyBinaryReader(BinaryReaderPtr binary);

#endif /* !BINARY_READER_H_ */
