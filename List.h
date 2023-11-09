#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

const int FREE_TESTICLE = -1;

typedef int elem_t;

struct ListNodes
{
    elem_t data;
    int next;
    int prev;
};

struct List
{
    int capacity;
    int size;
    int head;
    int tail;
    int free;
    ListNodes * nodes;
};

enum Errors
{
    NO_ERRORS               = 0 << 0,
    LIST_IS_NULL            = 1 << 0,
    CAPACITY_LESS_ONE       = 1 << 1,
    SIZE_IS_NEGATIVE        = 1 << 2,
    FREE_LESS_ONE           = 1 << 3,
    SIZE_MORE_CAPACITY      = 1 << 4,
    NODES_IS_NULL           = 1 << 5,
    CHANGE_FINCTON          = 1 << 6,
    INDEX_IS_FREE           = 1 << 7,
};

void list_ctor(List * list, const int INITIAL_CAPACITY);
void list_dtor(List * list);

int list_push_front(List * list, const elem_t value);
int list_push_back(List * list, const elem_t value);
int list_pop_front(List * list, elem_t * value);
int list_pop_back(List * list, elem_t * value);

int list_insert_after(List * list, const int index, const elem_t value);
int list_insert_before(List * list, const int index, const elem_t value);
int list_delete(List * list, const int index, elem_t * value);
int list_search(List * list, const int index);

#endif //LIST_H_INCLUDED
