#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

void list_ctor(List * lst)
{
    assert(lst != NULL);

    lst->size = 0;
    lst->capacity = INITIAL_CAPACITY;
    lst->free = 1;
    lst->head = 0;
    lst->tail = 0;

    lst->data = (elem_t *)calloc(lst->capacity, sizeof(elem_t));
    if(lst->data == NULL)
    {
        printf("Pointer on data is NULL\n");
        abort();
    }
    lst->data[0] = POISON;

    lst->next = (int *)calloc(lst->capacity, sizeof(int));
    if(lst->next == NULL)
    {
        printf("Pointer on next is NULL\n");
        abort();
    }
    for(int i = 1; i < lst->capacity; i++)
        lst->next[i] = FREE_TESTICLE;

    lst->prev = (int *)calloc(lst->capacity, sizeof(int));
    if(lst->prev == NULL)
    {
        printf("Pointer on prev is NULL\n");
        abort();
    }
    for(int i = 1; i < lst->capacity; i++)
        lst->prev[i] = FREE_TESTICLE;
}

void insert(List * lst, const int index, const elem_t value)
{
    assert(lst != NULL);
    assert(index >= 0);

    lst->data[lst->free] = value;
    lst->next[lst->free] = index;
    lst->prev[lst->free] = index - 1;

    lst->next[index - 1] = lst->free;
    lst->prev[index] = lst->free - 1;

    lst->free = lst->next[lst->free];
    lst->size++;
}

void push_front(List * lst, const elem_t value)
{
    assert(lst != NULL);

    lst->data[lst->free] = value;
    lst->next[lst->free] = lst->next[0];
    lst->prev[lst->free] = 0;

    lst->next[0] = lst->free;
    lst->prev[lst->head] = lst->free;

    lst->head = lst->free;

    if(lst->size == 0)
        lst->tail = lst->free;

    lst->size++;
    lst->free++;
}