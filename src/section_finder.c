#include "section_finder.h"
#include "binary_reader.h"
#include "def.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

static FNSTATUS
GetTextBaseAddress(SectionFinderPtr section)
{
    Elf32_Ehdr *ehdr = NULL;
    Elf32_Shdr *sects = NULL;
    Elf32_Shdr *shstrsect = NULL;
    char *shstrtab = NULL;
    int shnum = 0;
    int shstrndx = 0;

    assert(section != NULL);
    ehdr = (Elf32_Ehdr *)section->_binary->_buffer;
    assert(ehdr != NULL);
    sects = (Elf32_Shdr *)(section->_binary->_buffer + ehdr->e_shoff);
    assert(sects != NULL);
    shnum = ehdr->e_shnum;
    shstrndx = ehdr->e_shstrndx;
    shstrsect = &sects[shstrndx];
    assert(shstrsect != NULL);
    shstrtab = section->_binary->_buffer + shstrsect->sh_offset;
    assert(shstrtab != NULL);
    for(int i = 0; i < shnum; i++) {
        if(strcmp(shstrtab+sects[i].sh_name, ".text") == 0) {
            section->_textOffset = sects[i].sh_offset;
            section->_textSize = sects[i].sh_size;
        }
    }
    section->_entryPointAddr = ehdr->e_entry;
    return FN_SUCCESS;
}

SectionFinderPtr
InitSectionFinder(BinaryReaderPtr binary)
{
    SectionFinderPtr section = calloc(1, sizeof(SectionFinder));

    assert(binary && section);
    section->_binary = binary;
    assert(GetTextBaseAddress(section) == FN_SUCCESS);
    return section;
}

FNSTATUS
DestroySectionFinder(SectionFinderPtr section)
{
    if (!section)
        return FN_FAILURE;
    free(section);
    return FN_SUCCESS;
}
