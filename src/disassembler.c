#include "disassembler.h"
#include "base_instr_fmt.h"
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
        printf("== Opcode: %#0x\n", nbr & 0x7F);
        printf("== Func3: %#0x\n", (nbr >> 12) & 0x7);
        InstrNodePtr head = disassemble->_instrNode;
        while (head) {
            // I-Type encoding format check
            if (head->_instr._iType._opcode == (nbr & 0x7F) &&
                head->_instr._iType._func3 == ((nbr >> 12) & 0x7)) {
                head->_instr._iType._imm = (nbr >> 20 & 0xFFF);
                head->_instr._iType._rs1 = (nbr >> 15) & 0x1F;
                head->_instr._iType._rd = (nbr >> 7) & 0x1F;
                printf("== ITYPE INSTRUCTION FOUND!! %s x%d, x%d, %d\n",
                    head->_instr._name, head->_instr._iType._rd,
                    head->_instr._iType._rs1,
                    head->_instr._iType._imm);
                break;
            }
            // S-Type encoding format check
            if (head->_instr._sType._opcode == (nbr & 0x7F) &&
                head->_instr._sType._func3 == ((nbr >> 12) & 0x7)) {
                head->_instr._sType._rs1 = (nbr >> 15) & 0x1F;
                head->_instr._sType._rs2 = (nbr >> 20) & 0x1F;
                head->_instr._sType._imm_11_5 = (nbr >> 25) & 0x7;
                head->_instr._sType._imm_4_0 = (nbr >> 7) & 0x1F;
                printf("== STYPE INSTRUCTION FOUND!! %s x%d, %d(x%d)\n",
                    head->_instr._name,
                    head->_instr._sType._rs2,
                    ((head->_instr._sType._imm_11_5 << 5) |
                    head->_instr._sType._imm_4_0),
                    head->_instr._sType._rs1);
                break;
            }
            head = head->_next;
        }
        offset += 4LL;
    }
    return FN_SUCCESS;
}

static FNSTATUS
RegisterRV32Instruction(DisassemblerPtr disassembler)
{
    assert(disassembler != NULL);
    /// ADDI Instruction
    Instr instr = {
        ._name = "addi",
        ._type = INSTR_I_TYPE,
        ._iType = {
            ._func3 = 0x0,
            ._opcode = 0x13
        }
    };
    AddNewInstr(&disassembler->_instrNode, instr);
    /// SW Instruction
    Instr instr2 = {
        ._name = "sw",
        ._type = INSTR_S_TYPE,
        ._sType = {
            ._func3 = 0x2,
            ._opcode = 0x23
        },
    };
    printf("DEBUG: %x\n", instr2._iType._opcode);
    AddNewInstr(&disassembler->_instrNode, instr2);
    return FN_SUCCESS;
}

DisassemblerPtr
InitDisassembler(SectionFinderPtr section)
{
    DisassemblerPtr disassembler = calloc(1, sizeof(Disassembler));

    assert(section && disassembler);
    disassembler->_section = section;
    disassembler->_instrNode = calloc(1, sizeof(InstrNode));
    assert(disassembler->_instrNode != NULL);
    assert(RegisterRV32Instruction(disassembler) == FN_SUCCESS);
    assert(DisassemblerDisassemble(disassembler) == FN_SUCCESS);
    return disassembler;
}

FNSTATUS
DestroyDisassembler(DisassemblerPtr disassembler)
{
    if (!disassembler)
        return FN_NULL_PTR;
    if (disassembler->_instrNode) {
        FreeInstrNode(&disassembler->_instrNode);
        disassembler->_instrNode = NULL;
    }
    free(disassembler);
    return FN_SUCCESS;
}
