#include "disassembler.h"
#include "def.h"
#include "section_finder.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static FNSTATUS
DisassemblerDisassemble(DisassemblerPtr disassemble)
{
    char temp[4] = {0};
    uint64_t offset = 0LL;
    uint32_t nbr = 0L;

    printf("== Text section offset: %#0lx\n", disassemble->_section->_textOffset);
    printf("== Text section size: %#0lx\n", disassemble->_section->_textSize);
    printf("== Entry point address: %#0lx\n", disassemble->_section->_entryPointAddr);
    while (offset < disassemble->_section->_textSize) {
        for (size_t i = 0; i < 4; i++) {
            temp[i] = disassemble->_section->_binary->_buffer[
                disassemble->_section->_textOffset + offset + i];
        }
        nbr = *(uint32_t *)temp;
        printf("%#010lx %#010x\n", disassemble->_section->_textOffset + offset, nbr);
        offset += 4LL;
    }
    return FN_SUCCESS;
}

DisassemblerPtr
InitDisassembler(SectionFinderPtr section)
{
    DisassemblerPtr disassembler = calloc(1, sizeof(Disassembler));

    assert(section && disassembler);
    disassembler->_section = section;
    assert(DisassemblerDisassemble(disassembler) == FN_SUCCESS);
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
