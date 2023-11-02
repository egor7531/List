#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ListDump.h"

void list_dump(List * lst)
{
    const char * fileName = "Dump.txt";
    FILE * fp = fopen(fileName, "w");

    if(fp == NULL)
    {
        printf("Pointer on %s is NULL\n", fileName);
        abort();
    }

    int width = 5;

    fprintf(fp, "%*s", width, "index");
    for(int i = 0; i < lst->capacity; i++)
        fprintf(fp, "%*d", width, i);

    fprintf(fp, "\n" "%*s", width, "data");
    for(int i = 0; i < lst->capacity; i++)
        fprintf(fp, "%*d", width, lst->data[i]);

    fprintf(fp, "\n" "%*s", width, "next");
    for(int i = 0; i < lst->capacity; i++)
        fprintf(fp, "%*d", width, lst->next[i]);

    fprintf(fp, "\n" "%*s", width, "prev");
    for(int i = 0; i < lst->capacity; i++)
        fprintf(fp, "%*d", width, lst->prev[i]);

    fprintf(fp, "\n" "[free = %d]\n", lst->free);
    fprintf(fp, "<head = %d>\n", lst->head);
    fprintf(fp, "<tail = %d>\n", lst->tail);
    fprintf(fp, "size = %d\n", lst->size);
    fprintf(fp, "capacity = %d\n", lst->capacity);

    fclose(fp);
}
