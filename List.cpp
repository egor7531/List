#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "ListDump.h"

const int POISON = -69;
const int COEFF_INCREASE = 2;
const int COEFF_DECREASE = 2;

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

void list_realloc(List * list)
{
    list->nodes = (ListNode *)realloc(list->nodes, list->capacity * sizeof(ListNode));

    if(list->nodes  == NULL)
    {
        list->errors = NODES_IS_NULL;
        list_dump(list);
        return list_dtor(list);
    }

    fill_nodes(list);
}

int list_verificator(List * list)
{
    int err = NO_ERRORS;

    if(list == NULL)                       return LIST_IS_NULL;
    if(list->capacity < 1)                 err |= CAPACITY_LESS_ONE;
    if(list->size < 0)                     err |= SIZE_IS_NEGATIVE;
    if(list->size >= list->capacity)       err |= SIZE_MORE_CAPACITY;
    if(list->nodes == NULL)                return list->errors = err | NODES_IS_NULL;
    if(list->nodes[0].data != POISON)      err |= CHANGE_FINCTON;
    if(list->nodes[0].next < 0)            err |= HEAD_IS_NEGATIVE;
    if(list->nodes[0].prev < 0)            err |= TAIL_IS_NEGATIVE;

    list->errors |= err;

    return list->errors;
}

List list_ctor(const int initial_capacity)
{
    List list = {};
    list.capacity = initial_capacity;
    list.size = 0;
    list.free = 1;
    list.errors = NO_ERRORS;

    list_realloc(&list);
    list.nodes[0].data = POISON;
    list.nodes[0].next = 0;
    list.nodes[0].prev = 0;

    return list;
}

void list_dtor(List * list)
{
    list->size = 0;
    list->capacity = 0;
    list->free = 0;

    free(list->nodes);

    list->nodes = NULL;
}

int list_push_front(List * list, const elem_t value)
{
    return list_insert_before(list, list->nodes[0].next, value);
}

int list_push_back(List * list, const elem_t value)
{
    return list_insert_after(list, list->nodes[0].prev, value);
}

int list_pop_front(List * list, elem_t * value)
{
    return list_delete(list, list->nodes[0].next, value);
}

int list_pop_back(List * list, elem_t * value)
{
    return list_delete(list, list->nodes[0].prev, value);
}

int list_insert_before(List * list, const int index, const elem_t value)
{
    #ifdef LIST_PROTECTION
    int err = list_verificator(list);
    if(index < 0)
        err |= INDEX_LESS_ZERO;
    if(list->nodes[index].prev == FREE_TESTICLE)
        err |= INDEX_IS_FREE;

    if(list == NULL)
        list->errors = err;

    if(err > 0)
    {
        list_dump(list);
        list_dtor(list);
        return err;
    }
    #endif

    if(list->size + 1 == list->capacity)
    {
        list->capacity *= COEFF_INCREASE;
        list_realloc(list);
    }

    int free = list->free;
    list->free = list->nodes[free].next;

    list->nodes[free].data = value;
    list->nodes[free].next = index;
    list->nodes[free].prev = list->nodes[index].prev;

    list->nodes[list->nodes[index].prev].next = free;
    list->nodes[index].prev = free;

    list->size++;

    return free;
}

int list_insert_after(List * list, const int index, const elem_t value)
{
    #ifdef LIST_PROTECTION
    int err = list_verificator(list);
    if(index < 0)
        err |= INDEX_LESS_ZERO;
    if(list->nodes[index].prev == FREE_TESTICLE)
        err |= INDEX_IS_FREE;

    if(list == NULL)
        list->errors = err;

    if(err > 0)
    {
        list_dump(list);
        list_dtor(list);
        return err;
    }
    #endif

    if(list->size + 1 == list->capacity)
    {
        list->capacity *= COEFF_INCREASE;
        list_realloc(list);
    }

    int free = list->free;
    list->free = list->nodes[free].next;

    list->nodes[free].data = value;
    list->nodes[free].next = list->nodes[index].next;
    list->nodes[free].prev = index;

    list->nodes[list->nodes[index].next].prev = free;
    list->nodes[index].next = free;

    list->size++;

    return free;
}

int list_delete(List * list, const int index, elem_t * value)
{
    #ifdef LIST_PROTECTION
    int err = list_verificator(list);
    if(value == NULL)
        err |= VALUE_IS_NULL;

    if(list == NULL)
        list->errors = err;

    if(err > 0)
    {
        list_dump(list);
        list_dtor(list);
        return err;
    }
    #endif

    *value = list->nodes[index].data;

    list->nodes[list->nodes[index].prev].next = list->nodes[index].next;
    list->nodes[list->nodes[index].next].prev = list->nodes[index].prev;

    list->nodes[index].data = 0;
    list->nodes[index].next = list->free;
    list->nodes[index].prev = FREE_TESTICLE;

    list->free = index;
    list->size--;

    return index;
}

int list_search(List * list, const int logilacIndex)
{
    #ifdef LIST_PROTECTION
    int err = list_verificator(list);
    if(logilacIndex < 0)
        err |= INDEX_LESS_ZERO;

    if(list == NULL)
        list->errors = err;

    if(err > 0)
    {
        list_dump(list);
        list_dtor(list);
        return err;
    }
    #endif

    int fisicalIndex = 0;
    if(logilacIndex <= list->size / 2)
    {
        fisicalIndex = list->nodes[0].next;
        for(int j = 1; j != logilacIndex; j++)
            fisicalIndex = list->nodes[fisicalIndex].next;
    }
    else
    {
        fisicalIndex = list->nodes[0].prev;
        for(int j = list->size; j != logilacIndex; j--)
            fisicalIndex = list->nodes[fisicalIndex].prev;
    }

    return fisicalIndex;
}
