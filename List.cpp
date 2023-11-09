#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

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

void list_ctor(List * list, const int INITIAL_CAPACITY)
{
    assert(list != NULL);
    assert(INITIAL_CAPACITY > 0);

    list->capacity = INITIAL_CAPACITY;
    list->size = 0;
    list->free = 1;
    list->head = 0;
    list->tail = 0;

    list->nodes = (ListNodes *)calloc(INITIAL_CAPACITY, sizeof(ListNodes));

    if(list->nodes  == NULL)
    {
        printf("Pointer on data is NULL\n");
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
        //else if(list->size *  == list->capacity)     -какое условие для уменьшения?

        list->nodes = (ListNodes *)realloc(list->nodes, list->capacity * sizeof(ListNodes));
                                                    // recalloc ?
        if(list->nodes  == NULL)
        {
            printf("Pointer on nodes is NULL\n");
            abort();
        }

        fill_nodes(list);
    }
}

int list_push_front(List * list, const elem_t value)
{
    assert(list != NULL);

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
    assert(list != NULL);

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
    assert(list != NULL);
    assert(value != NULL);

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
    assert(list != NULL);
    assert(value != NULL);

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
    assert(list != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    list_realloc_if_need(list);

    if(index == list->head)
        return list_push_front(list, value);

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
    assert(list != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    list_realloc_if_need(list);

    if(index == list->tail)
        return list_push_back(list, value);

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
    assert(list != NULL);
    assert(index >= 0);
    assert(value != NULL);

    if(index == list->head)
        return list_pop_front(list, value);
    else if(index == list->tail)
        return list_pop_back(list, value);

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

int list_search(List * list, const int index)
{
    assert(list != NULL);
    assert(index >= 0);

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

int list_verificator(List * list)
{
    int error = NO_ERRORS;

    if(list == NULL)                                        return LIST_IS_NULL;
    if(list->capacity < 1)                                  error |= CAPACITY_LESS_ONE;
    if(list->size < 0)                                      error |= SIZE_IS_NEGATIVE;
    if(list->free < 1)                                      error |= FREE_LESS_ONE;
    if(list->size >= list->capacity)                        error |= SIZE_MORE_CAPACITY;
    if(list->nodes == NULL)                                 return error |= NODES_IS_NULL;
    if(list->nodes[0].data != POISON)                       error |= CHANGE_FINCTON;

    return error;
}
