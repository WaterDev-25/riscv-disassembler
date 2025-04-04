#include "def.h"
#include "binary_reader.h"
#include "disassembler.h"
#include "section_finder.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

FNSTATUS
main(int argc, char **argv)
{
    BinaryReaderPtr binary = NULL;
    SectionFinderPtr section = NULL;
    DisassemblerPtr disassembler = NULL;

    if (argc < 2) {
        printf("-- Argument is missing.\n");
        exit(1);
    }
    binary = InitBinaryReader(argv[1]);
    if (!binary)
        exit(1);
    section = InitSectionFinder(binary);
    if (!section)
        exit(1);
    disassembler = InitDisassembler(section);
    assert(binary && section && disassembler);
    assert(SUCCESS(DestroyDisassembler(disassembler)));
    assert(SUCCESS(DestroyBinaryReader(binary)));
    assert(SUCCESS(DestroySectionFinder(section)));
    return 0x00000000;
}
