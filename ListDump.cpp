#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ListDump.h"

void list_dump(List * list)
{
    assert(list != NULL);

    const char * fileName = "Dump.txt";
    FILE * fp = fopen(fileName, "wb");

    if(fp == NULL)
    {
        printf("Pointer on %s is NULL\n", fileName);
        abort();
    }

    int width = 5;

    fprintf(fp, "%*s", width, "index");
    for(int i = 0; i < list->capacity; i++)
        fprintf(fp, "%*d", width, i);

    fprintf(fp, "\n" "%*s", width, "data");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->head)
            fprintf(fp, "%*d>", width, list->node[i].data);
        else if(i == list->tail)
            fprintf(fp, "%*d]", width, list->node[i].data);
        else
            fprintf(fp, "%*d", width, list->node[i].data);
    }

    fprintf(fp, "\n" "%*s", width, "next");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->head)
            fprintf(fp, "%*d>", width, list->node[i].next);
        else if(i == list->tail)
            fprintf(fp, "%*d]", width, list->node[i].next);
        else
            fprintf(fp, "%*d", width, list->node[i].next);
    }

    fprintf(fp, "\n" "%*s", width, "prev");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->head)
            fprintf(fp, "%*d>", width, list->node[i].prev);
        else if(i == list->tail)
            fprintf(fp, "%*d]", width, list->node[i].prev);
        else
            fprintf(fp, "%*d", width, list->node[i].prev);
    }

    fprintf(fp, "\n" "free = %d\n", list->free);
    fprintf(fp, "<head = %d>\n", list->head);
    fprintf(fp, "[tail = %d]\n", list->tail);
    fprintf(fp, "size = %d\n", list->size);
    fprintf(fp, "capacity = %d\n", list->capacity);

    fclose(fp);
}

