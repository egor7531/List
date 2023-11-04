#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef int elem_t;

struct ListNode
{
    elem_t * data;
    int * next;
    int * prev;
};

struct List
{
    int capacity;
    int size;
    int head;
    int tail;
    int free;
    ListNode node;
};

void list_ctor(List * list);
void list_dtor(List * list);

int push_front(List * list, const elem_t value);
int push_back(List * list, const elem_t value);
int pop_front(List * list, elem_t * value);
int pop_back(List * list, elem_t * value);

int list_insert_after(List * list, const int index, const elem_t value);
int list_insert_before(List * list, const int index, const elem_t value);
int list_delete(List * list, const int index, elem_t * value);
int list_search(List * list, const int index);

#endif //LIST_H_INCLUDED
