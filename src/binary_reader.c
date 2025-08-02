#include "binary_reader.h"
#include "def.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static STATUS
ReadBinaryFile( BinaryReaderPtr binary )
{
    assert(binary != NULL);
    FILE *fp = fopen(binary->_fileName, "rb");
    size_t fz = 0L;
    size_t rz = 0L;

    UNUSED(fz);
    if (!fp) {
        printf("-- Impossible to read the given binary.\n");
        return STATUS_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    fz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    binary->_buffer = calloc(fz, sizeof(char));
    assert(binary->_buffer != NULL);
    rz = fread(binary->_buffer, sizeof(char), fz, fp);
    if (rz != fz) {
        printf("-- Binary reader: failed to read %s.\n", binary->_fileName);
        fclose(fp);
        return STATUS_FAILURE;
    }
    printf("-- Binary reader: %s IS_SUCCESSfully readed!\n", binary->_fileName);
    fclose(fp);
    return STATUS_SUCCESS;
}

BinaryReaderPtr
InitBinaryReader( const char *fileName )
{
    BinaryReaderPtr binary = calloc(1, sizeof(BinaryReader));

    assert(fileName != NULL && binary != NULL);
    binary->_fileName = (char *)fileName;
    if (!IS_SUCCESS(ReadBinaryFile(binary))) {
        DestroyBinaryReader(binary);
        return NULL;
    }
    return binary;
}

STATUS
DestroyBinaryReader( BinaryReaderPtr binary )
{
    assert(binary != NULL);
    if (binary->_buffer) {
        free(binary->_buffer);
        binary->_buffer = NULL;
    }
    free(binary);
    return STATUS_SUCCESS;
}
