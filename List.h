#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef int elem_t;

struct List
{
    int capacity;
    int size;
    int head;
    int tail;
    int free;
    elem_t * data;
    int * next;
    int * prev;
};

const int INITIAL_CAPACITY = 10;
const int POISON = -69;
const int FREE_TESTICLE = -1;

void list_ctor(List * lst);
void list_dtor(List * lst);
int push_front(List * lst, const elem_t value);
int push_back(List * lst, const elem_t value);
int pop_front(List * lst, elem_t * value);
int pop_back(List * lst, elem_t * value);
int list_insert_after(List * lst, const int index, const elem_t value);
int list_insert_before(List * lst, const int index, const elem_t value);
int list_delete(List * lst, const int index, elem_t * value);
int list_search(List * lst, const int index);

#endif //LIST_H_INCLUDED
