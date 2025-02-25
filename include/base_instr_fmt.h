#ifndef BASE_INSTR_FMT_H_
    #define BASE_INSTR_FMT_H_

    #include "def.h"
    #include <stdint.h>

typedef struct {
    uint16_t _imm : 12;
    uint8_t _rs1 : 5;
    uint8_t _func3 : 3;
    uint8_t _rd : 5;
    uint8_t _opcode : 7;
} IType, *ITypePtr;

typedef struct {
    uint8_t _imm_11_5 : 7;
    uint8_t _rs2 : 5;
    uint8_t _rs1 : 5;
    uint8_t _func3 : 3;
    uint8_t _imm_4_0 : 5;
    uint8_t _opcode : 7;
} SType, *STypePtr;

typedef struct {
    uint32_t _imm : 20;
    uint8_t _rd : 4;
    uint8_t _opcode : 7;
} UType, *UTypePtr;

typedef struct {
    uint8_t _imm_20 : 1;
    uint16_t _imm_10_1 : 10;
    uint8_t _imm_11 : 1;
    uint32_t _imm_19_12 : 8;
    uint8_t _rd : 4;
    uint8_t _opcode : 7;
} JType, *JTypePtr;

typedef enum {
    INSTR_I_TYPE,
    INSTR_S_TYPE,
    INSTR_U_TYPE,
    INSTR_J_TYPE
} InstrType, *InstrTypePtr;

typedef struct {
    char *_name;
    InstrType _type;
    //union {
        IType _iType;
        SType _sType;
        UType _uType;
        JType _jType;
    //} _format;
} Instr, *InstrPtr;

typedef struct InstrNode {
    Instr _instr;
    struct InstrNode *_next;
} InstrNode, *InstrNodePtr, **InstrNodeDPtr;

FNSTATUS AddNewInstr(InstrNodeDPtr head, Instr instr);
FNSTATUS FreeInstrNode(InstrNodeDPtr head);

#endif /* !BASE_INSTR_FMT_H_ */
