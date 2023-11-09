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
        fclose(fp);
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
            fprintf(fp, "%*d>", width, list->nodes[i].data);
        else if(i == list->tail)
            fprintf(fp, "%*d]", width, list->nodes[i].data);
        else
            fprintf(fp, "%*d", width, list->nodes[i].data);
    }

    fprintf(fp, "\n" "%*s", width, "next");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->head)
            fprintf(fp, "%*d>", width, list->nodes[i].next);
        else if(i == list->tail)
            fprintf(fp, "%*d]", width, list->nodes[i].next);
        else
            fprintf(fp, "%*d", width, list->nodes[i].next);
    }

    fprintf(fp, "\n" "%*s", width, "prev");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->head)
            fprintf(fp, "%*d>", width, list->nodes[i].prev);
        else if(i == list->tail)
            fprintf(fp, "%*d]", width, list->nodes[i].prev);
        else
            fprintf(fp, "%*d", width, list->nodes[i].prev);
    }

    fprintf(fp, "\n" "free = %d\n"
                "<head = %d>\n"
                "[tail = %d]\n"
                "size = %d\n"
                "capacity = %d\n",
                list->free, list->head, list->tail, list->size,list->capacity);

    fclose(fp);
}

void list_graphic_dump(List * list)
{
    assert(list != NULL);
    assert(list->nodes != NULL);

    const char * nameFile = "Dump.dot";
    FILE * fp = fopen(nameFile, "wb");

    if(fp == NULL)
    {
        printf("Can't open file: %s\n", nameFile);
        fclose(fp);
        abort();
    }

    fprintf(fp, "digraph G\n"
                "{\n"
                "rankdir = LR;\n"
                "node [style = \"filled\", shape = \"record\", color = \"#000000\"];\n"
                "size [fillcolor = \"#FF69B4\", label = \" size = %d \"];\n"
                "capacity [fillcolor = \"#FF69B4\", label = \" capacity = %d \"];\n"
                "free [fillcolor = \"#FF69B4\", label = \" free = %d \"];\n"
                "head [fillcolor = \"#FF69B4\", label = \" head = %d \"];\n"
                "tail [fillcolor = \"#FF69B4\", label = \" tail = %d \"];\n"
                "node0 [fillcolor = \"#FF0000\" label = "
                " \" index: 0 | data: %d | <next> next: %d | <prev> prev: %d \"];\n",
                list->size, list->capacity, list->free, list->head, list->tail,
                list->nodes[0].data, list->nodes[0].next, list->nodes[0].prev);

    for(int i = 1; i < list->capacity; i++)
    {
        fprintf(fp, "node%d ", i);

        if(list->nodes[list->capacity - 1 - i].prev != FREE_TESTICLE)
            fprintf(fp, "[fillcolor = \"#00FFFF\", ");
        else
            fprintf(fp, "[fillcolor = \"#ADFF2F\", ");

        fprintf(fp, "label = "
                    " \" index: %d | data: %d | <next> next: %d | <prev> prev: %d \"];\n",
                    i, list->nodes[i].data, list->nodes[i].next, list->nodes[i].prev);
    }

    fprintf(fp, "edge [style = \"invis\"];\n");
    for(int i = 0; i < list->capacity - 1; i++)
        fprintf(fp, "node%d -> node%d;\n", i, i + 1);

    fprintf(fp, "edge [style = \"solid\"];\n");
    for(int i = 0; i < list->capacity; i++)
    {
        if(list->nodes[list->capacity - 1 - i].prev != FREE_TESTICLE)
            fprintf(fp, "node%d :<prev> -> node%d :<prev> [color = \"#FF0000\"];\n"
                        "node%d :<next> -> node%d :<next> [color = \"#0000FF\"];\n",
                        list->capacity - 1 - i, list->nodes[list->capacity - 1 - i].prev,
                        i, list->nodes[i].next);
        else
            fprintf(fp, "node%d :<next> -> node%d :<next> [color = \"#FF00FF\"];\n",
                list->capacity - 1 - i, list->nodes[list->capacity - 1 - i].next);
    }

    fprintf(fp, "edge [color = \"#000000\", constraint = false];\n"
                "free -> node%d;\n"
                "head -> node%d;\n"
                "tail -> node%d;\n"
                "}",
                list->free, list->head, list->tail);

    fclose(fp);
}
