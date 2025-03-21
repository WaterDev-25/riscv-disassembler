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
       // printf("%#010lx %#010x\n", disassemble->_section->_textOffset + offset, nbr);
        InstrNodePtr head = disassemble->_instrNode;
        while (head) {
            // I-Type encoding format check
            if (head->_instr._iType._opcode == (nbr & 0x7F) &&
                head->_instr._iType._func3 == ((nbr >> 12) & 0x7)) {
                head->_instr._iType._imm = (nbr >> 20 & 0xFFF);
                head->_instr._iType._rs1 = (nbr >> 15) & 0x1F;
                head->_instr._iType._rd = (nbr >> 7) & 0x1F;
                printf("%#010lx\t%s x%d, x%d, %d\n",
                    disassemble->_section->_textOffset + offset,
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
                printf("%#010lx\t%s x%d, %d(x%d)\n",
                    disassemble->_section->_textOffset + offset,
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
                printf("%#010lx\t%s x%d, %d\n",
                    disassemble->_section->_textOffset + offset,
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
                printf("%#010lx\t%s x%d, %d\n",
                    disassemble->_section->_textOffset + offset,
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
                break;
            }
            // R-Type encoding format check
            if (head->_instr._rType._opcode == (nbr & 0x7F) &&
                head->_instr._rType._func3 == ((nbr >> 12) & 0x7) &&
                head->_instr._rType._func7 == ((nbr >> 26) & 0x7F)) {
                head->_instr._rType._rs2 = (nbr >> 20) & 0x1F;
                head->_instr._rType._rs1 = (nbr >> 15) & 0x1F;
                head->_instr._rType._rd = (nbr >> 7) & 0x1F;
                printf("%#010lx\t%s x%d, x%d, x%d\n",
                    disassemble->_section->_textOffset + offset,
                    head->_instr._name,
                    head->_instr._rType._rd,
                    head->_instr._rType._rs1,
                    head->_instr._rType._rs2);
                break;
            }
            // B-Type encoding format check
            if (head->_instr._bType._opcode == (nbr & 0x7F) &&
                head->_instr._bType._func3 == ((nbr >> 12) & 0x7)) {
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
    Instr instructions[] = {
        // I TYPE
        { ._name = "addi", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x13 } },
        { ._name = "slti", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x13, ._func3 = 0x2 } },
        { ._name = "sltiu", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x13, ._func3 = 0x3 } },
        { ._name = "xori", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x13, ._func3 = 0x4 } },
        { ._name = "ori", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x13, ._func3 = 0x5 } },
        { ._name = "andi", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x13, ._func3 = 0x7 } },
        { ._name = "jalr", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x67 } },
        { ._name = "lb", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x3 } },
        { ._name = "lh", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x3, ._func3 = 0x1 } },
        { ._name = "lw", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x3, ._func3 = 0x2 } },
        { ._name = "lbu", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x3, ._func3 = 0x3 } },
        { ._name = "lhu", ._type = INSTR_I_TYPE, ._iType = { ._opcode = 0x3, ._func3 = 0x4 } },
        // S Type
        { ._name = "sb", ._type = INSTR_S_TYPE, ._sType = { ._opcode = 0x23 } },
        { ._name = "sh", ._type = INSTR_S_TYPE, ._sType = { ._opcode = 0x23, ._func3 = 0x1 } },
        { ._name = "sw", ._type = INSTR_S_TYPE, ._sType = { ._opcode = 0x23, ._func3 = 0x2 } },
        // U Type
        { ._name = "lui", ._type = INSTR_U_TYPE, ._uType = { ._opcode = 0x37 } },
        { ._name = "auipc", ._type = INSTR_U_TYPE, ._uType = { ._opcode = 0x17 } },
        // J Type
        { ._name = "jal", ._type = INSTR_J_TYPE, ._jType = { ._opcode = 0x6F } },
        // R Type
        { ._name = "add", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33 } },
        { ._name = "xor", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func3 = 0x4 } },
        { ._name = "or", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func3 = 0x6 } },
        { ._name = "sll", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func3 = 0x1 } },
        { ._name = "slt", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func3 = 0x2 } },
        { ._name = "srl", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func3 = 0x5 } },
        { ._name = "sra", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func3 = 0x5, ._func7 = 0x20 } },
        { ._name = "sltu", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func3 = 0x3 } },
        { ._name = "sub", ._type = INSTR_R_TYPE, ._rType = { ._opcode = 0x33, ._func7 = 0x20 } },
        // B Type
        { ._name = "beq", ._type = INSTR_B_TYPE, ._bType = { ._opcode = 0x63 } },
        { ._name = "bne", ._type = INSTR_B_TYPE, ._bType = { ._opcode = 0x63, ._func3 = 0x1 } },
        { ._name = "blt", ._type = INSTR_B_TYPE, ._bType = { ._opcode = 0x63, ._func3 = 0x4 } },
        { ._name = "bge", ._type = INSTR_B_TYPE, ._bType = { ._opcode = 0x63, ._func3 = 0x5 } },
         { ._name = "bltu", ._type = INSTR_B_TYPE, ._bType = { ._opcode = 0x63, ._func3 = 0x6 } },
         { ._name = "bgeu", ._type = INSTR_B_TYPE, ._bType = { ._opcode = 0x63, ._func3 = 0x7 } }
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
