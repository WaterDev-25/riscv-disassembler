#include "section_finder.h"
#include "binary_reader.h"
#include "def.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

static STATUS
GetTextBaseAddress( SectionFinderPtr section )
{
    char *buffer = NULL;
    int shnum = 0;
    int shstrndx = 0;
    Elf32_Shdr *sects = NULL;
    Elf32_Shdr *shstrsect = NULL;
    char *shstrtab = NULL;

    if (!section || !section->_binary || !section->_binary->_buffer)
        return STATUS_FAILURE;
    buffer = section->_binary->_buffer;
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)buffer;
    if (ehdr->e_ident[0] != 0x7f ||
        ehdr->e_ident[1] != 'E' ||
        ehdr->e_ident[2] != 'L' ||
        ehdr->e_ident[3] != 'F') {
        printf("-- The given binary is not an elf file.\n");
        return STATUS_FAILURE;
    }
    sects = (Elf32_Shdr *)(buffer + ehdr->e_shoff);
    shnum = ehdr->e_shnum;
    shstrndx = ehdr->e_shstrndx;
    if (shnum <= 0 || shstrndx < 0 || shstrndx >= shnum)
        return STATUS_FAILURE;
    shstrsect = &sects[shstrndx];
    shstrtab = (char *)(buffer + shstrsect->sh_offset);
    if (!shstrtab)
        return STATUS_FAILURE;
    for (int i = 0; i < shnum; i++) {
        if (strcmp(shstrtab + sects[i].sh_name, ".text") == 0) {
            section->_textOffset = sects[i].sh_offset;
            section->_textSize = sects[i].sh_size;
        }
    }
    section->_entryPointAddr = ehdr->e_entry;
    return STATUS_SUCCESS;
}

SectionFinderPtr
InitSectionFinder( BinaryReaderPtr binary )
{
    SectionFinderPtr section = NULL;

    assert(binary != NULL);
    section = calloc(1, sizeof(SectionFinder));
    assert(section != NULL);
    section->_binary = binary;
    if (!IS_SUCCESS(GetTextBaseAddress(section))) {
        free(section);
        return NULL;
    }
    return section;
}

STATUS
DestroySectionFinder( SectionFinderPtr section )
{
    assert(section != NULL);
    free(section);
    return STATUS_SUCCESS;
}
