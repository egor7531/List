#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

const int INITIAL_CAPACITY = 5;
const int POISON = -69;
const int FREE_TESTICLE = -1;
const int COEFF_INCREASE = 2;
const int COEFF_DECREASE = 2;

void fill_next_and_prev(List * list)
{
    assert(list != NULL);

    for(int i = list->free; i < list->capacity; i++)
        list->node[i].next= i + 1;

    for(int i = list->free; i < list->capacity; i++)
        list->node[i].prev = FREE_TESTICLE;
}

void list_ctor(List * list)
{
    assert(list != NULL);

    list->capacity = INITIAL_CAPACITY;
    list->size = 0;
    list->free = 1;
    list->head = 0;
    list->tail = 0;

    list->node = (ListNode *)calloc(list->capacity, sizeof(ListNode));

    if(list->node  == NULL)
    {
        printf("Pointer on data is NULL\n");
        abort();
    }

    list->node[0].data = POISON;

    fill_next_and_prev(list);
}

void list_dtor(List * list)
{
    list->size = 0;
    list->capacity = 0;
    list->free = 0;
    list->head = 0;
    list->tail = 0;

    free(list->node);

    list->node = NULL;
}

void if_need_realloc(List * list)
{
    assert(list != NULL);

    if(list->size + 1 == list->capacity)
    {
        list->capacity *= COEFF_INCREASE;
        //else if(list->size *  == list->capacity)     -какое условие для уменьшения?

        list->node = (ListNode *)realloc(list->node, list->capacity * sizeof(ListNode));

        if(list->node  == NULL)
        {
            printf("Pointer on data is NULL\n");
            abort();
        }

        fill_next_and_prev(list);
    }
}

int push_front(List * list, const elem_t value)
{
    assert(list != NULL);

    int free = list->free;

    if_need_realloc(list);

    list->free = list->node[free].next;

    list->node[free].data = value;
    list->node[free].next = list->node[0].next;
    list->node[free].prev = 0;

    list->node[0].next = free;
    list->node[list->head].prev = free;
    list->head = free;
    list->size++;

    if(list->size == 1)
        list->tail = free;

    return list->head;
}

int push_back(List * list, const elem_t value)
{
    assert(list != NULL);

    int free = list->free;

    if_need_realloc(list);

    list->free = list->node[free].next;

    list->node[free].data = value;
    list->node[free].next = 0;
    list->node[free].prev = list->node[0].prev;

    list->node[list->tail].next = free;
    list->node[0].prev = free;
    list->tail = free;
    list->size++;

    if(list->size == 1)
        list->head = free;

    return list->tail;
}

int pop_front(List * list, elem_t * value)
{
    assert(list != NULL);
    assert(value != NULL);

    *value = list->node[list->head].data;

    int free = list->free;

    list->free = list->head;
    list->head = list->node[list->head].next;

    list->node[list->free].data = 0;
    list->node[list->free].next = free;
    list->node[list->free].prev = FREE_TESTICLE;

    list->node[0].next = list->head;
    list->node[list->head].prev = 0;

    list->size--;

    if(list->size == 0)
        list->tail = 0;

    return list->free;
}

int pop_back(List * list, elem_t * value)
{
    assert(list != NULL);
    assert(value != NULL);

    *value = list->node[list->tail].data;

    int free = list->free;

    list->free = list->tail;
    list->tail = list->node[list->tail].prev;

    list->node[list->free].data = 0;
    list->node[list->free].next = free;
    list->node[list->free].prev = FREE_TESTICLE;

    list->node[list->tail].next = 0;
    list->node[0].prev = list->tail;

    list->size--;

    if(list->size == 0)
        list->head = 0;

    return list->free;
}

int list_insert_before(List * list, const int index, const elem_t value)
{
    assert(list != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    if_need_realloc(list);

    if(index == list->head)
        return push_front(list, value);

    list->node[list->free].data = value;
    list->node[list->free].next = index;
    list->node[list->free].prev = list->node[index].prev;

    list->node[list->node[index].prev].next = list->free;
    list->node[index].prev = list->free;

    int free = list->free;
    list->free = list->node[list->free].next;
    list->size++;

    return free;
}

int list_insert_after(List * list, const int index, const elem_t value)
{
    assert(list != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    if_need_realloc(list);

    if(index == list->tail)
        return push_back(list, value);

    list->node[list->free].data = value;
    list->node[list->free].next = list->node[index].next;
    list->node[list->free].prev = index;

    list->node[list->node[index].next].prev = list->free;
    list->node[index].next = list->free;

    int free = list->free;
    list->free = list->node[list->free].next;
    list->size++;

    return free;
}

int list_delete(List * list, const int index, elem_t * value)
{
    assert(list != NULL);
    assert(index >= 0);
    assert(value != NULL);

    if(index == list->head)
        return pop_front(list, value);
    else if(index == list->tail)
        return pop_back(list, value);

    *value = list->node[index].data;

    list->node[list->node[index].prev].next = list->node[index].next;
    list->node[list->node[index].next].prev = list->node[index].prev;

    list->node[index].data = 0;
    list->node[index].next = list->free;
    list->node[index].prev = FREE_TESTICLE;

    list->free = index;
    list->size--;

    return list->free;
}

int list_search(List * list, const int index)
{
    assert(list != NULL);
    assert(index >= 0);

    int i = 0;
    if(index <= list->size / 2)
    {
        i = list->node[0].next;
        for(int j = 1; j != index; j++)
            i = list->node[i].next;
    }
    else
    {
        i = list->node[0].prev;
        for(int j = list->size; j != index; j--)
            i = list->node[i].prev;
    }

    return i;
}
