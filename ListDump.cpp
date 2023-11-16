#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ListDump.h"

void print_errors(FILE *fp, const int err)
{
    assert(fp != nullptr);
    assert(err >= 0);

    if(err == NO_ERRORS)
        fprintf(fp, "No errors");

    for(int i = 1; i < err; i *= 2)
    {
        switch(err & i)
        {
            case LIST_IS_NULL: fprintf(fp, "Pointer on List is null");
                break;
            case CAPACITY_LESS_ONE: fprintf(fp, "Pointer on List is null");
                break;
            case HEAD_IS_NEGATIVE: fprintf(fp, "Head of list is negative");
                break;
            case TAIL_IS_NEGATIVE: fprintf(fp, "Tail of list is negative");
                break;
            case SIZE_IS_NEGATIVE: fprintf(fp, "Size of list is negative");
                break;
            case FREE_LESS_ONE: fprintf(fp, "Index of free element less 1");
                break;
            case SIZE_MORE_CAPACITY: fprintf(fp, "Size more capacity");
                break;
            case NODES_IS_NULL: fprintf(fp, "Pointer on struct of nodes is null");
                break;
            case CHANGE_FINCTON: fprintf(fp, "Value of finction elemt change");
                break;
            case INDEX_LESS_ONE: fprintf(fp, "Pointer on element of list less 1");
                break;
            case INDEX_IS_FREE: fprintf(fp, "Pointer on element of list indicate on free element");
                break;
            case VALUE_IS_NULL: fprintf(fp, "Pointer on value of argument is null");
                break;
            default:
                break;
        }
    }
}
void list_dump(const List * list)
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

    fprintf(fp, "\n" "free = %d\n"
                "<head = %d>\n"
                "[tail = %d]\n"
                "size = %d\n"
                "capacity = %d\n",
                list->free, list->nodes[0].next, list->nodes[0].prev, list->size,list->capacity);
    fprintf(fp, "status error: ");
    print_errors(fp, list->errors);

    fclose(fp);
}

void list_graphic_dump(const List * list)
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
}
