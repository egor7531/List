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

int push_front(List * lst, const elem_t value)
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

    return lst->free - 1;
}

int push_back(List * lst, const elem_t value)
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

    return lst->free - 1;
}

int pop_front(List * lst, elem_t * value)
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

    return lst->free;
}

int pop_back(List * lst, elem_t * value)
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

    return lst->free;
}

int list_insert_before(List * lst, const int index, const elem_t value)
{
    assert(lst != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    if(index == lst->head)
        return push_front(lst, value);

    lst->data[lst->free] = value;
    lst->next[lst->free] = index;
    lst->prev[lst->free] = lst->prev[index];

    lst->next[lst->prev[index]] = lst->free;
    lst->prev[index] = lst->free;

    lst->free++;
    lst->size++;

    return lst->free - 1;
}

int list_insert_after(List * lst, const int index, const elem_t value)
{
    assert(lst != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    if(index == lst->tail)
        return push_back(lst, value);

    lst->data[lst->free] = value;
    lst->next[lst->free] = lst->next[index];
    lst->prev[lst->free] = index;

    lst->prev[lst->next[index]] = lst->free;
    lst->next[index] = lst->free;

    lst->free++;
    lst->size++;

    return lst->free - 1;
}

int list_delete(List * lst, const int index, elem_t * value)
{
    assert(lst != NULL);
    assert(index >= 0);
    assert(value != NULL);

    if(index == lst->head)
        return pop_front(lst, value);
    else if(index == lst->tail)
        return pop_back(lst, value);

    *value = lst->data[index];

    lst->free = index;

    lst->next[lst->prev[index]] = lst->next[index];
    lst->prev[lst->next[index]] = lst->prev[index];

    lst->data[index] = 0;
    lst->next[index] = FREE_TESTICLE;
    lst->prev[index] = FREE_TESTICLE;

    lst->size--;

    return lst->free;
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

