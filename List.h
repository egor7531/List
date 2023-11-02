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
const int BOUNDARY_LIST = 0;

void list_ctor(List * lst);
void insert(List * lst, const int index, const int value);
void push_front(List * lst, const elem_t value);

#endif //LIST_H_INCLUDED
