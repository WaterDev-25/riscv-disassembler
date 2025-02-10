#ifndef DISASSEMBLER_H_
    #define DISASSEMBLER_H_

    #include "def.h"
    #include "section_finder.h"

typedef struct {
    SectionFinderPtr _section;
} Disassembler, *DisassemblerPtr;

DisassemblerPtr InitDisassembler(SectionFinderPtr section);
FNSTATUS DestroyDisassembler(DisassemblerPtr disassembler);

#endif /* !DISASSEMBLER_H_ */
