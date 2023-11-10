#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "ListDump.h"

//#define PROTECTION

/*#define DEF_CHECK_LIST
    #ifdef PROTECTION                   \
    list_verificator(list);             \
    if(list->errors > 0)                \
    {                                   \
        list_dump(list);                \
        abort();                        \
    }                                   \
    #endif
*/

const int POISON = -69;
const int COEFF_INCREASE = 2;
const int COEFF_DECREASE = 2;

int list_verificator(List * list)
{
    int err = list->errors;

    if(list == NULL)                                        return LIST_IS_NULL;
    if(list->capacity < 1)                                  err |= CAPACITY_LESS_ONE;
    if(list->head < 1)                                      err |= HEAD_LESS_ONE;
    if(list->tail < 1)                                      err |= TAIL_LESS_ONE;
    if(list->size < 0)                                      err |= SIZE_IS_NEGATIVE;
    if(list->free < 1)                                      err |= FREE_LESS_ONE;
    if(list->size >= list->capacity)                        err |= SIZE_MORE_CAPACITY;
    if(list->nodes == NULL)                                 return err |= NODES_IS_NULL;
    if(list->nodes[0].data != POISON)                       err |= CHANGE_FINCTON;

    list->errors = err;
}

void fill_nodes(List * list)
{
    assert(list != NULL);

    for(int i = list->free; i < list->capacity; i++)
    {
        list->nodes[i].data = 0;
        list->nodes[i].next = i + 1;
        list->nodes[i].prev = FREE_TESTICLE;
    }
}

void list_ctor(List * list, const int INITIAL_CAPACITY)
{
    assert(list != NULL);

    list->capacity = INITIAL_CAPACITY;
    list->size = 0;
    list->free = 1;
    list->head = 1;
    list->tail = 1;
    list->errors = NO_ERRORS;

    list->nodes = (ListNodes *)calloc(INITIAL_CAPACITY, sizeof(ListNodes));

    if(list->nodes  == NULL)
    {
        list->errors = NODES_IS_NULL;
        list_dump(list);
        abort();
    }

    list->nodes[0].data = POISON;

    fill_nodes(list);
}

void list_dtor(List * list)
{
    list->size = 0;
    list->capacity = 0;
    list->free = 0;
    list->head = 0;
    list->tail = 0;

    free(list->nodes);

    list->nodes = NULL;
}

void list_realloc_if_need(List * list)
{
    assert(list != NULL);

    if(list->size + 1 == list->capacity)
    {
        list->capacity *= COEFF_INCREASE;

        list->nodes = (ListNodes *)realloc(list->nodes, list->capacity * sizeof(ListNodes));

        if(list->nodes  == NULL)
        {
            list->errors = NODES_IS_NULL;
            list_dump(list);
            abort();
        }

        fill_nodes(list);
    }
}

int list_push_front(List * list, const elem_t value)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }
    #endif

    list_realloc_if_need(list);

    int free = list->free;
    list->free = list->nodes[free].next;

    list->nodes[free].data = value;
    list->nodes[free].next = list->nodes[0].next;
    list->nodes[free].prev = 0;

    list->nodes[0].next = free;
    list->nodes[list->head].prev = free;
    list->head = free;
    list->tail = list->nodes[0].prev;
    list->size++;

    return list->head;
}

int list_push_back(List * list, const elem_t value)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }
    #endif

    list_realloc_if_need(list);

    int free = list->free;
    list->free = list->nodes[free].next;

    list->nodes[free].data = value;
    list->nodes[free].next = 0;
    list->nodes[free].prev = list->nodes[0].prev;

    list->nodes[list->tail].next = free;
    list->nodes[0].prev = free;
    list->tail = free;
    list->head = list->nodes[0].next;
    list->size++;

    return list->tail;
}

int list_pop_front(List * list, elem_t * value)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(value == NULL)
        list->errors = VALUE_IS_NULL;

    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }
    #endif

    *value = list->nodes[list->head].data;

    int free = list->free;

    list->free = list->head;
    list->head = list->nodes[list->head].next;

    list->nodes[list->free].data = 0;
    list->nodes[list->free].next = free;
    list->nodes[list->free].prev = FREE_TESTICLE;

    list->nodes[0].next = list->head;
    list->nodes[list->head].prev = 0;

    list->tail = list->nodes[0].prev;

    list->size--;

    return list->free;
}

int list_pop_back(List * list, elem_t * value)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(value == NULL)
        list->errors = VALUE_IS_NULL;

    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }
    #endif

    *value = list->nodes[list->tail].data;

    int free = list->free;

    list->free = list->tail;
    list->tail = list->nodes[list->tail].prev;

    list->nodes[list->free].data = 0;
    list->nodes[list->free].next = free;
    list->nodes[list->free].prev = FREE_TESTICLE;

    list->nodes[list->tail].next = 0;
    list->nodes[0].prev = list->tail;

    list->head = list->nodes[0].next;

    list->size--;

    return list->free;
}

int list_insert_before(List * list, const int index, const elem_t value)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(index < 1)
        list->errors = INDEX_LESS_ONE;

    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }

    if(list->nodes[index].prev == FREE_TESTICLE)
    {
        list->errors = INDEX_IS_FREE;
        list_dump(list);
        abort();
    }
    #endif

    list_realloc_if_need(list);

    int free = list->free;
    list->free = list->nodes[free].next;

    list->nodes[free].data = value;
    list->nodes[free].next = index;
    list->nodes[free].prev = list->nodes[index].prev;

    list->nodes[list->nodes[index].prev].next = free;
    list->nodes[index].prev = free;

    list->head = list->nodes[0].next;
    list->tail = list->nodes[0].prev;

    list->size++;

    return free;
}

int list_insert_after(List * list, const int index, const elem_t value)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(index < 1)
        list->errors = INDEX_LESS_ONE;

    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }

    if(list->nodes[index].prev == FREE_TESTICLE)
    {
        list->errors = INDEX_IS_FREE;
        list_dump(list);
        abort();
    }
    #endif

    list_realloc_if_need(list);

    int free = list->free;
    list->free = list->nodes[free].next;

    list->nodes[free].data = value;
    list->nodes[free].next = list->nodes[index].next;
    list->nodes[free].prev = index;

    list->nodes[list->nodes[index].next].prev = free;
    list->nodes[index].next = free;

    list->head = list->nodes[0].next;
    list->tail = list->nodes[0].prev;

    list->size++;

    return free;
}

int list_delete(List * list, const int index, elem_t * value)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(value == NULL)
        list->errors = VALUE_IS_NULL;

    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }
    #endif

    *value = list->nodes[index].data;

    list->nodes[list->nodes[index].prev].next = list->nodes[index].next;
    list->nodes[list->nodes[index].next].prev = list->nodes[index].prev;

    list->nodes[index].data = 0;
    list->nodes[index].next = list->free;
    list->nodes[index].prev = FREE_TESTICLE;

    list->head = list->nodes[0].next;
    list->tail = list->nodes[0].prev;

    list->free = index;
    list->size--;

    return index;
}

int list_search(List * list, const int index)
{
    #ifdef PROTECTION
    list_verificator(list);
    if(index < 1)
        list->errors = INDEX_LESS_ONE;

    if(list->errors > 0)
    {
        list_dump(list);
        abort();
    }
    #endif

    int i = 0;
    if(index <= list->size / 2)
    {
        i = list->nodes[0].next;
        for(int j = 1; j != index; j++)
            i = list->nodes[i].next;
    }
    else
    {
        i = list->nodes[0].prev;
        for(int j = list->size; j != index; j--)
            i = list->nodes[i].prev;
    }

    return i;
}
