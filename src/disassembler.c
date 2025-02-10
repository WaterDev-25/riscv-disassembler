#include "disassembler.h"
#include "def.h"
#include "section_finder.h"
#include <assert.h>
#include <stdlib.h>

DisassemblerPtr
InitDisassembler(SectionFinderPtr section)
{
    DisassemblerPtr disassembler = calloc(1, sizeof(Disassembler));

    assert(section && disassembler);
    disassembler->_section = section;
    return disassembler;
}

FNSTATUS
DestroyDisassembler(DisassemblerPtr disassembler)
{
    if (!disassembler)
        return FN_NULL_PTR;
    free(disassembler);
    return FN_SUCCESS;
}
