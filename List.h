#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#define LIST_PROTECTION

const int FREE_TESTICLE = -1;

typedef int elem_t;

struct ListNode
{
    elem_t data;
    int next;
    int prev;
};

struct List
{
    int capacity;
    int size;
    int free;
    int errors;
    ListNode * nodes;
};

enum List_Errors
{
    NO_ERRORS               = 0 << 0,
    LIST_IS_NULL            = 1 << 0,
    CAPACITY_LESS_ONE       = 1 << 1,
    HEAD_IS_NEGATIVE        = 1 << 2,
    TAIL_IS_NEGATIVE        = 1 << 3,
    SIZE_IS_NEGATIVE        = 1 << 4,
    FREE_LESS_ONE           = 1 << 5,
    SIZE_MORE_CAPACITY      = 1 << 6,
    NODES_IS_NULL           = 1 << 7,
    CHANGE_FINCTON          = 1 << 8,
    INDEX_LESS_ZERO         = 1 << 9,
    INDEX_IS_FREE           = 1 << 10,
    VALUE_IS_NULL           = 1 << 11,
    FP_IS_NULL              = 1 << 12,
    ERROR_WORK_SYSTEM       = 1 << 13
};

List list_ctor(const int initial_capacity);
void list_dtor(List* list);

int list_push_front(List*  list, const elem_t value);
int list_push_back(List* list, const elem_t value);
int list_pop_front(List* list, elem_t * value);
int list_pop_back(List* list, elem_t * value);

int list_insert_after(List* list, const int index, const elem_t value);
int list_insert_before(List* list, const int index, const elem_t value);
int list_delete(List* list, const int index, elem_t* value);

int list_search(List* list, const int logilacIndex);

#endif //LIST_H_INCLUDED
