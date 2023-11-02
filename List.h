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
void push_front(List * lst, const elem_t value);
void push_back(List * lst, const elem_t value);
void pop_front(List * lst, elem_t * value);
void pop_back(List * lst, elem_t * value);
void insert(List * lst, const int index, const int value);

#endif //LIST_H_INCLUDED
