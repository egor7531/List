#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListDump.h"

void print_errors(FILE* fp, const int err)
{
    assert(fp != nullptr);
    assert(err >= 0);

    if(err == NO_ERRORS)
        fprintf(fp, "No errors");
    if(err & LIST_IS_NULL)
        fprintf(fp, "Pointer on List is null");
    if(err & CAPACITY_LESS_ONE)
        fprintf(fp, "Pointer on List is null");
    if(err & HEAD_IS_NEGATIVE)
        fprintf(fp, "Head of list is negative");
    if(err & TAIL_IS_NEGATIVE)
        fprintf(fp, "Tail of list is negative");
    if(err & SIZE_IS_NEGATIVE)
        fprintf(fp, "Size of list is negative");
    if(err & FREE_LESS_ONE)
        fprintf(fp, "Index of free element less 1");
    if(err & SIZE_MORE_CAPACITY)
        fprintf(fp, "Size more capacity");
    if(err & NODES_IS_NULL)
        fprintf(fp, "Pointer on struct of nodes is null");
    if(err & CHANGE_FINCTON)
        fprintf(fp, "Value of finction elemt change");
    if(err & INDEX_LESS_ZERO)
        fprintf(fp, "Pointer on element of list less 1");
    if(err & INDEX_IS_FREE)
        fprintf(fp, "Pointer on element of list indicate on free element");
    if(err & VALUE_IS_NULL)
        fprintf(fp, "Pointer on value of argument is null");
    if(err & FP_IS_NULL)
        fprintf(fp, "Pointer on file if NULL");
    if(err & ERROR_WORK_SYSTEM)
        fprintf(fp, "Erroneous operation of the systems funcion");
}

void list_dump(List* list)
{
    assert(list != NULL);

    if(list->nodes == NULL)
    {
        list->errors |= NODES_IS_NULL;
        return;
    }

    const char* fileName = "Dump.txt";
    FILE* fp = fopen(fileName, "wb");
    if(fp == NULL)
    {
        list->errors |= FP_IS_NULL;
        fclose(fp);
        return;
    }

    fprintf(fp, "free = %d\n"
                "<head = %d>\n"
                "[tail = %d]\n"
                "size = %d\n"
                "capacity = %d\n",
                list->free, list->nodes[0].next, list->nodes[0].prev, list->size,list->capacity);
    fprintf(fp, "status error: ");
    print_errors(fp, list->errors);

    int width = 5;

    fprintf(fp, "%*s", width, "index");
    for(int i = 0; i < list->capacity; i++)
        fprintf(fp, "%*d", width, i);

    fprintf(fp, "\n" "%*s", width, "data");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->nodes[0].next)
            fprintf(fp, "%*d>", width, list->nodes[i].data);
        else if(i == list->nodes[0].prev)
            fprintf(fp, "%*d]", width, list->nodes[i].data);
        else
            fprintf(fp, "%*d", width, list->nodes[i].data);
    }

    fprintf(fp, "\n" "%*s", width, "next");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->nodes[0].next)
            fprintf(fp, "%*d>", width, list->nodes[i].next);
        else if(i == list->nodes[0].prev)
            fprintf(fp, "%*d]", width, list->nodes[i].next);
        else
            fprintf(fp, "%*d", width, list->nodes[i].next);
    }

    fprintf(fp, "\n" "%*s", width, "prev");
    for(int i = 0; i < list->capacity; i++)
    {
        if(i == list->nodes[0].next)
            fprintf(fp, "%*d>", width, list->nodes[i].prev);
        else if(i == list->nodes[0].prev)
            fprintf(fp, "%*d]", width, list->nodes[i].prev);
        else
            fprintf(fp, "%*d", width, list->nodes[i].prev);
    }

    fclose(fp);
}

void list_system(List* list, const char* nameFilePng, const char* nameFileDot)
{
    assert(list != NULL);

    const int MAX_SIZE_COMMAND = 100;
    char command[MAX_SIZE_COMMAND] = "dot ";
    strncat(command, nameFileDot, MAX_SIZE_COMMAND);
    strncat(command, " -T png -o ", MAX_SIZE_COMMAND);
    strncat(command, nameFilePng, MAX_SIZE_COMMAND);

    if(system(command) > 0)
    {
        list->errors |= ERROR_WORK_SYSTEM;
        return;
    }
}

void list_graphic_dump(List* list, const char* nameFilePng)
{
    assert(list != NULL);

    if(list->nodes == NULL)
    {
        list->errors |= NODES_IS_NULL;
        return;
    }

    const char* nameFileDot = "Dump.dot";
    FILE* fp = fopen(nameFileDot, "wb");
    if(fp == NULL)
    {
        list->errors |= FP_IS_NULL;
        fclose(fp);
        return;
    }

    fprintf(fp, "digraph List\n"
                "{\n"
                "rankdir = LR;\n"
                "node [style = \"filled\", shape = \"record\", color = \"#000000\"];\n"
                "info [fillcolor = \"#FF69B4\", label = "
                " \" capacity = %d | size = %d | status errors = ",
                list->capacity, list->size);
    print_errors(fp, list->errors);
    fprintf(fp, " \"];\n");

    for(int i = 0; i < list->capacity - 1; i++)
        fprintf(fp, "node%d -> ", i);
    fprintf(fp, "node%d [style = \"invis\"];\n",list->capacity - 1);

    fprintf(fp, "{\n"
                "node0 [fillcolor = \"#FF0000\", label = "
                " \" index: 0 | data: %d | <next> next: %d | <prev> prev: %d \"];\n",
                list->nodes[0].data, list->nodes[0].next, list->nodes[0].prev);
    for(int i = 1; i < list->capacity; i++)
    {
        fprintf(fp, "node%d ", i);

        if(list->nodes[i].prev != FREE_TESTICLE)
            fprintf(fp, "[fillcolor = \"#00FFFF\", ");
        else
            fprintf(fp, "[fillcolor = \"#ADFF2F\", ");

        fprintf(fp, "label = "
                    " \" index: %d | data: %d | <next> next: %d | <prev> prev: %d \"];\n",
                    i, list->nodes[i].data, list->nodes[i].next, list->nodes[i].prev);
    }

    fprintf(fp, "edge [style = \"solid\", constraint = false];\n");
    for(int i = 0; i < list->capacity; i++)
    {
        if(list->nodes[i].prev != FREE_TESTICLE)
            fprintf(fp, //"node%d :<prev> -> node%d :<prev> [color = \"#FF0000\"];\n"
                        "node%d -> node%d [color = \"#000000\"];\n",
                        //list->nodes[i].prev, i,
                        i, list->nodes[i].next);
        else if(list->nodes[i].next != list->capacity)
            fprintf(fp, "node%d -> node%d [color = \"#00FF00\"];\n",
                i, list->nodes[i].next);
    }
    fprintf(fp, "}");

    fprintf(fp, "free [shape = \"rect\" fillcolor = \"#ADFF2F\"];\n"
                "head [shape = \"rect\" fillcolor = \"#00FFFF\"];\n"
                "tail [shape = \"rect\" fillcolor = \"#00FFFF\"];\n"
                "{ rank = same; node%d; free; }\n"
                "{ rank = same; node%d; head; }\n"
                "{ rank = same; node%d; tail; };\n",
                list->free, list->nodes[0].next, list->nodes[0].prev);

    fprintf(fp, "}");
    fclose(fp);
    list_system(list, nameFilePng, nameFileDot);
}
