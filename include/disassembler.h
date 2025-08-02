#ifndef DISASSEMBLER_H_
    #define DISASSEMBLER_H_

    #include "base_instr_fmt.h"
    #include "def.h"
    #include "section_finder.h"

typedef struct {
    SectionFinderPtr _section;
    InstrNodePtr     _instrNode;
} Disassembler, *DisassemblerPtr;

DisassemblerPtr InitDisassembler( SectionFinderPtr section );
STATUS DestroyDisassembler( DisassemblerPtr disassembler );

#endif /* !DISASSEMBLER_H_ */
