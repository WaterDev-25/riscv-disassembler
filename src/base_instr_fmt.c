#include "base_instr_fmt.h"
#include "def.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

FNSTATUS
AddNewInstr(InstrNodeDPtr head, Instr instr)
{
    InstrNodePtr new_node = NULL; 

    assert(head != NULL);
    new_node = calloc(1, sizeof(InstrNode));
    assert(new_node != NULL);
    new_node->_instr = instr;
    new_node->_next = *head;
    *head = new_node;
    return FN_SUCCESS;
}

FNSTATUS
FreeInstrNode(InstrNodeDPtr head)
{
    InstrNodePtr temp = NULL;

    assert(head != NULL);
    while (*head) {
        temp = (*head)->_next;
        free(*head);
        *head = NULL;
        *head = temp;
    }
    return FN_SUCCESS;
}
