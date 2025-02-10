#include "def.h"
#include "binary_reader.h"
#include "disassembler.h"
#include "section_finder.h"
#include <assert.h>

PRSTATUS
main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    BinaryReaderPtr binary = InitBinaryReader("tests/return");
    SectionFinderPtr section = InitSectionFinder(binary);
    DisassemblerPtr disassembler = InitDisassembler(section);
    assert(binary && section && disassembler);

    FNSTATUS status = DestroyDisassembler(disassembler);
    assert(status == FN_SUCCESS);
    status = DestroyBinaryReader(binary);
    assert(status == FN_SUCCESS);
    status = DestroySectionFinder(section);
    assert(status == FN_SUCCESS);
    return PR_SUCCESS;
}
