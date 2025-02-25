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
            // U-Type encoding format check
            if (head->_instr._uType._opcode == (nbr & 0x7F)) {
                head->_instr._uType._imm = (nbr >> 12) & 0xFFFFF;
                head->_instr._uType._rd = (nbr >> 7) & 0x1F;
                printf("== UTYPE INSTRUCTION FOUND!! %s x%d, %d\n",
                    head->_instr._name,
                    head->_instr._uType._rd,
                    head->_instr._uType._imm);
                break;
            }
            // J-Type encoding format check
            if (head->_instr._jType._opcode == (nbr & 0x7F)) {
                head->_instr._jType._imm_19_12 = (nbr >> 12) & 0xFF;
                head->_instr._jType._imm_11 = (nbr >> 20) & 0x1;
                head->_instr._jType._imm_10_1 = (nbr >> 21) & 0x3FF; 
                head->_instr._jType._imm_20 = (nbr >> 31) & 0x1;
                head->_instr._jType._rd = (nbr >> 7) & 0x1F;
                printf("== JTYPE INSTRUCTION FOUND!! %s x%d, %d\n",
                    head->_instr._name,
                    head->_instr._jType._rd,
                    head->_instr._jType._imm_20 ? ((head->_instr._jType._imm_20 << 20) |
                    (head->_instr._jType._imm_19_12 << 12) |
                    (head->_instr._jType._imm_11 << 11) |
                    (head->_instr._jType._imm_10_1 << 1)) | (int32_t)0xFFE00000 :
                    ((head->_instr._jType._imm_20 << 20) |
                    (head->_instr._jType._imm_19_12 << 12) |
                    (head->_instr._jType._imm_11 << 11) |
                    (head->_instr._jType._imm_10_1 << 1)));
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
    Instr instructions[] = {
        { ._name = "addi", ._type = INSTR_I_TYPE, ._iType = { ._func3 = 0x0, ._opcode = 0x13 } },
        { ._name = "sw", ._type = INSTR_S_TYPE, ._sType = { ._func3 = 0x2, ._opcode = 0x23 } },
        { ._name = "lui", ._type = INSTR_U_TYPE, ._uType = { ._opcode = 0x37 } },
        { ._name = "auipc", ._type = INSTR_U_TYPE, ._uType = { ._opcode = 0x17 } },
        { ._name = "jal", ._type = INSTR_J_TYPE, ._jType = { ._opcode = 0x6F } }
    };

    for (size_t i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++)
        AddNewInstr(&disassembler->_instrNode, instructions[i]);
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
