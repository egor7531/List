#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "List.h"
#include "ListDump.h"

int main()
{
    List lst = {};
    list_ctor(&lst);
    list_dump(&lst);

    while(true)
    {
        printf("end: 0\n");
        printf("push_front: 1\n");
        printf("push_back: 2\n");
        printf("pop_front: 3\n");
        printf("pop_back: 4\n");
        printf("insert: 5\n");
        printf("delate: 6\n");
        printf("Enter number of command: \n");
        int cmd = 0;
        scanf("%d", &cmd);

        elem_t x = 0;
        if(cmd == 1)
        {
            scanf("%d", &x);
            push_front(&lst, x);
        }
        else if(cmd == 0)
            break;

        list_dump(&lst);
    }

    return 0;
}
