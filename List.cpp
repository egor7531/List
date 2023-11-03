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

void list_dtor(List * lst)
{
    lst->size = 0;
    lst->capacity = 0;
    lst->free =0;
    lst->head = 0;
    lst->tail = 0;

    free(lst->data);
    free(lst->next);
    free(lst->prev);

    lst->data = NULL;
    lst->next = NULL;
    lst->prev = NULL;
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
    lst->size++;

    if(lst->size == 1)
        lst->tail = lst->free;

    lst->free++;
}

void push_back(List * lst, const elem_t value)
{
    assert(lst != NULL);

    lst->data[lst->free] = value;
    lst->next[lst->free] = 0;
    lst->prev[lst->free] = lst->prev[0];

    lst->next[lst->tail] = lst->free;
    lst->prev[0] = lst->free;
    lst->tail = lst->free;
    lst->size++;

    if(lst->size == 1)
        lst->head = lst->free;

    lst->free++;
}

void pop_front(List * lst, elem_t * value)
{
    assert(lst != NULL);
    assert(value != NULL);

    *value = lst->data[lst->head];

    lst->free = lst->head;
    lst->head = lst->next[lst->head];

    lst->data[lst->free] = 0;
    lst->next[lst->free] = FREE_TESTICLE;
    lst->prev[lst->free] = FREE_TESTICLE;

    lst->next[0] = lst->head;
    lst->prev[lst->head] = 0;

    lst->size--;

    if(lst->size == 0)
        lst->tail = 0;
}

void pop_back(List * lst, elem_t * value)
{
    assert(lst != NULL);
    assert(value != NULL);

    *value = lst->data[lst->tail];

    lst->free = lst->tail;
    lst->tail = lst->prev[lst->tail];

    lst->data[lst->free] = 0;
    lst->next[lst->free] = FREE_TESTICLE;
    lst->prev[lst->free] = FREE_TESTICLE;

    lst->next[lst->tail] = 0;
    lst->prev[0] = lst->tail;

    lst->size--;

    if(lst->size == 0)
        lst->head = 0;
}

void list_insert(List * lst, const int index, const elem_t value)
{
    assert(lst != NULL);
    assert(index >= 0);                    //Как обрабатывать случай, когда index > size?

    int i = list_search(lst, index);

    if(i == lst->head)
        push_front(lst, value);
    else if(i == lst->tail)
        push_back(lst, value);
    else
    {
        lst->data[lst->free] = value;
        lst->next[lst->free] = i;
        lst->prev[lst->free] = lst->prev[i];

        lst->next[lst->prev[i]] = lst->free;
        lst->prev[i] = lst->free;

        lst->free++;
        lst->size++;
    }
}

void list_delete(List * lst, const int index, elem_t * value)
{
    assert(lst != NULL);
    assert(index >= 0);
    assert(value != NULL);

    int i = list_search(lst, index);

    if(i == lst->head)
        pop_front(lst, value);
    else if(i == lst->tail)
        pop_back(lst, value);
    else
    {
        *value = lst->data[i];

        lst->free = i;

        lst->next[lst->prev[i]] = lst->next[i];
        lst->prev[lst->next[i]] = lst->prev[i];

        lst->data[i] = 0;
        lst->next[i] = FREE_TESTICLE;
        lst->prev[i] = FREE_TESTICLE;

        lst->size--;
    }
}

int list_search(List * lst, const int index)
{
    assert(lst != NULL);
    assert(index >= 0);

    int i = 0;
    if(index <= lst->size / 2)
    {
        i = lst->next[0];
        for(int j = 1; j != index; j++)
            i = lst->next[i];
    }
    else
    {
        i = lst->prev[0];
        for(int j = lst->size; j != index; j--)
            i = lst->prev[i];
    }

    return i;
}

