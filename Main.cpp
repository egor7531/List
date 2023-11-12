#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "List.h"
#include "ListDump.h"

int main()
{
    List list = {};
    const int INITIAL_CAPACITY = 5;
    list_ctor(&list, INITIAL_CAPACITY);
    list_dump(&list);
    /*while(true)
    {
        printf("end: 0\n");
        printf("push_front: 1\n");
        printf("push_back: 2\n");
        printf("pop_front: 3\n");
        printf("pop_back: 4\n");
        printf("insert after: 5\n");
        printf("insert before: 6\n");
        printf("delate: 7\n");
        printf("Enter number of command: \n");
        int cmd = 0;
        scanf("%d", &cmd);

        elem_t x = 0;
        if(cmd == 1)
        {
            printf("Enter x:");
            scanf("%d", &x);
            list_push_front(&list, x);
        }
        else if(cmd == 2)
        {
            printf("Enter x:");
            scanf("%d", &x);
            list_push_back(&list, x);
        }
        else if(cmd == 3)
        {
            list_pop_front(&list, &x);
            printf("\n" "x = %d\n", x);
        }
        else if(cmd == 4)
        {
            list_pop_back(&list, &x);
            printf("\n" "x = %d\n", x);
        }
        else if(cmd == 5)
        {
            printf("Enter i:");
            int i = 0;
            scanf("%d", &i);
            printf("Enter x:");
            scanf("%d", &x);
            list_insert_after(&list, i, x);
        }
        else if(cmd == 6)
        {
            printf("Enter i:");
            int i = 0;
            scanf("%d", &i);
            printf("Enter x:");
            scanf("%d", &x);
            list_insert_before(&list, i, x);
        }
        else if(cmd == 7)
        {
            printf("Enter i:");
            int i = 0;
            scanf("%d", &i);
            list_delete(&list, i, &x);
            printf("\n" "x = %d\n", x);
        }
        else if(cmd == 0)
            break;

        list_dump(&list);
    }*/

    int x = 0;
    list_push_front(&list, 10);
    list_push_back(&list, 30);
    for(int i = 1; i < 6; i++)
        list_insert_after(&list, i, 50);

    list_push_front(&list, 1);
    list_pop_back(&list, &x);

    list_dump(&list);
    list_graphic_dump(&list);
    list_dtor(&list);

    return 0;
}
