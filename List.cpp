#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

const int INITIAL_CAPACITY = 5;
const int POISON = -69;
const int FREE_TESTICLE = -1;

void list_ctor(List * list)
{
    assert(list != NULL);

    list->capacity = INITIAL_CAPACITY;
    list->size = 0;
    list->free = 1;
    list->head = 0;
    list->tail = 0;

    list->node.data = (elem_t *)calloc(list->capacity,3 * sizeof(elem_t));

    if(list->node.data  == NULL)
    {
        printf("Pointer on data is NULL\n");
        abort();
    }

    list->node.next = list->node.data + list->capacity;
    list->node.prev = list->node.data + 2 * list->capacity;

    list->node.data[0] = POISON;

    for(int i = 1; i < list->capacity; i++)
        list->node.next[i] = i + 1;

    for(int i = 1; i < list->capacity; i++)
        list->node.prev[i] = FREE_TESTICLE;
}

void list_dtor(List * list)
{
    list->size = 0;
    list->capacity = 0;
    list->free = 0;
    list->head = 0;
    list->tail = 0;

    free(list->node.data);
    free(list->node.next);
    free(list->node.prev);

    list->node.data = NULL;
    list->node.next = NULL;
    list->node.prev = NULL;
}

int push_front(List * list, const elem_t value)
{
    assert(list != NULL);

    int free = list->free;
    list->free = list->node.next[free];

    list->node.data[free] = value;
    list->node.next[free] = list->node.next[0];
    list->node.prev[free] = 0;

    list->node.next[0] = free;
    list->node.prev[list->head] = free;
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
    list->free = list->node.next[free];

    list->node.data[free] = value;
    list->node.next[free] = 0;
    list->node.prev[free] = list->node.prev[0];

    list->node.next[list->tail] = free;
    list->node.prev[0] = free;
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

    *value = list->node.data[list->head];

    int free = list->free;

    list->free = list->head;
    list->head = list->node.next[list->head];

    list->node.data[list->free] = 0;
    list->node.next[list->free] = free;
    list->node.prev[list->free] = FREE_TESTICLE;

    list->node.next[0] = list->head;
    list->node.prev[list->head] = 0;

    list->size--;

    if(list->size == 0)
        list->tail = 0;

    return list->free;
}

int pop_back(List * list, elem_t * value)
{
    assert(list != NULL);
    assert(value != NULL);

    *value = list->node.data[list->tail];

    int free = list->free;

    list->free = list->tail;
    list->tail = list->node.prev[list->tail];

    list->node.data[list->free] = 0;
    list->node.next[list->free] = free;
    list->node.prev[list->free] = FREE_TESTICLE;

    list->node.next[list->tail] = 0;
    list->node.prev[0] = list->tail;

    list->size--;

    if(list->size == 0)
        list->head = 0;

    return list->free;
}

int list_insert_before(List * list, const int index, const elem_t value)
{
    assert(list != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    if(index == list->head)
        return push_front(list, value);

    list->node.data[list->free] = value;
    list->node.next[list->free] = index;
    list->node.prev[list->free] = list->node.prev[index];

    list->node.next[list->node.prev[index]] = list->free;
    list->node.prev[index] = list->free;

    int free = list->free;
    list->free = list->node.next[list->free];
    list->size++;

    return free;
}

int list_insert_after(List * list, const int index, const elem_t value)
{
    assert(list != NULL);
    assert(index >= 0);               //Как обрабатывать случай, когда index > size? - ошибка

    if(index == list->tail)
        return push_back(list, value);

    list->node.data[list->free] = value;
    list->node.next[list->free] = list->node.next[index];
    list->node.prev[list->free] = index;

    list->node.prev[list->node.next[index]] = list->free;
    list->node.next[index] = list->free;

    int free = list->free;
    list->free = list->node.next[list->free];
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

    *value = list->node.data[index];

    list->node.next[list->node.prev[index]] = list->node.next[index];
    list->node.prev[list->node.next[index]] = list->node.prev[index];

    list->node.data[index] = 0;
    list->node.next[index] = list->free;
    list->node.prev[index] = FREE_TESTICLE;

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
        i = list->node.next[0];
        for(int j = 1; j != index; j++)
            i = list->node.next[i];
    }
    else
    {
        i = list->node.prev[0];
        for(int j = list->size; j != index; j--)
            i = list->node.prev[i];
    }

    return i;
}
