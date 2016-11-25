#include <stdio.h>
#include <stdlib.h>

//#define DEBUG

#define N 100000


typedef struct list list;

struct list
{
    int val;
    int len;
    list* next;
    list* prev;
    list* root;
};


list* new_list(int val)
{
    list* l = (list*)malloc(sizeof(list));
    l->val = val;
    l->len = 1;
    l->next = NULL;
    l->prev = NULL;
    l->root = l;
    return l;
}

list* rotate_right(list* l)
{
    #ifdef DEBUG
        printf("Rotating right on %p\n", l);
    #endif
    list* cur = l->root;
    int len = cur->len;
    if(cur->prev)
    {
        cur->root = cur->prev;
        cur->root->len = len;
        cur->prev->root = cur->root;
    }
    return cur->root;
}

list* rotate_left(list* l)
{
    #ifdef DEBUG
        printf("Rotating left on %p\n", l);
    #endif
    list* cur = l->root;
    int len = cur->len;
    if(cur->next)
    {
        cur->root = cur->next;
        cur->root->len = len;
        cur->next->root = cur->root;
    }
    return cur->root;
}

list* push(list* l, int v)
{
    if(!l)
        return new_list(v);

    list* maillon = new_list(v);
    list* cur = l->root;
    int len = cur->len;
    if(cur->prev)
    {
        maillon->prev = cur->prev;
        maillon->next = cur;
        maillon->prev->next = maillon;
        cur->prev = maillon;
    }
    else
    {
        cur->prev = maillon;
        cur->next = maillon;
        maillon->prev = cur;
        maillon->next = cur;
    }
    cur->root = maillon;
    cur->root->len = 1+len;
    return cur->root;
}

list* pop_root(list** l)
{
    list* cur = (*l)->root;

    *l = cur->next;
    if(*l)
        (*l)->len = cur->len-1;

    cur->len = 1;

    if(cur->prev == cur)
        *l = NULL;
    if(cur->prev)
    {
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        cur->prev->root = cur->next;
        cur->next->root = cur->next;
        cur->prev = NULL;
        cur->next = NULL;
    }
    else
        *l = NULL;

    return cur;
}

int length(list* l)
{
    if(l)
        return l->root->len;
    else
        return 0;
}

void print_list(list* l)
{
    if(!l)
    {
        printf("NULL\n");
        return;
    }
    list* r = l->root;
    printf("%d : ", length(r));
    printf("%d ", r->val);
    while(r->next && r->next != l->root)
    {
        r = r->next;
        printf("%d ", r->val);
    }
    printf("\n");
}

void free_list(list* l)
{
    if(!l)
        return;

    list* r = l->root;
    while(r->next && r->next != l->root)
    {
        list* t = r->next;
        free(r);
        r = t;
    }
}

int pop_push(list** pop_on, list** push_on)
{
    #ifdef DEBUG
        printf("Popping on %p pushing on %p\n", *pop_on, *push_on);
    #endif
    list* tmp = pop_root(pop_on);
    *push_on = push(*push_on, tmp->val);
    free(tmp);

    return *pop_on != NULL;
}

int get_top(list* l)
{
    if(l)
        return l->root->val;
    else
        return -1;
}

/** testing lines

int main()
{
    list* l = new_list(10);
    print_list(l);
    l = push(l,20);
    l = push(l,1);
    l = push(l,3);
    list* tmp = pop_root(&l);
    print_list(l);
    l = rotate_left(l);
    print_list(l);
    l = rotate_left(l);
    print_list(l);
    l = rotate_right(l);
    print_list(l);
    print_list(tmp);
    list* tmp2 = pop_root(&tmp);
    print_list(tmp2);

    int t = pop_push(&l,&tmp2);
    while(t)
    {
        printf("=================\n");
        print_list(l);
        print_list(tmp2);
        t = pop_push(&l,&tmp2);
    }
    printf("=================\n");
    print_list(l);
    print_list(tmp2);

    free_list(l);
    free_list(tmp2);
}
**/

#include <time.h>

//gcc fast_sort.c -o fast_sort

int main()
{
    srand(time(NULL));

    list* l1 = new_list(rand()%1000000);
    list* l2 = NULL;

    int i=0;
    for(;i<N;i++)
        l1 = push(l1, rand()%1000000);

    #ifdef DEBUG
        print_list(l1);
        print_list(l2);
    #endif

    int total = length(l1);
    list* piv_list = NULL;
    int n_operations = 0;
    for(int j=0; j<total || piv_list != NULL; j++)
    {
        int n = 0;
        while(l1 != NULL)
        {
            int len = length(l1);
            int piv = get_top(l1);
            #ifdef DEBUG
                printf("Chosen piv : %d\n", piv);
            #endif
            n_operations++;
            pop_push(&l1, &l2);
            #ifdef DEBUG
                printf("=============\n");
                print_list(l1);
                print_list(l2);
            #endif
            n = 0;
            for(int i=1; i<len; i++)
            {
                if(get_top(l1) >= piv)
                {
                    pop_push(&l1, &l2);
                    n++;
                }
                else
                    l1 = rotate_left(l1);
                #ifdef DEBUG
                    printf("=============\n");
                    print_list(l1);
                    print_list(l2);
                #endif
                n_operations++;
            }
            piv_list = push(piv_list, n);
        }

        list* cur = pop_root(&piv_list);

        for(int i=0; i<cur->val; i++)
        {
            pop_push(&l2, &l1);
            n_operations++;
            #ifdef DEBUG
                printf("=============\n");
                print_list(l1);
                print_list(l2);
            #endif
        }
        l2 = rotate_left(l2);
        n_operations++;
        #ifdef DEBUG
            printf("=============\n");
            print_list(l1);
            print_list(l2);
        #endif
    }

    printf("======RESULT=======\n");
    print_list(l1);
    print_list(l2);

    printf("Sort done in %d operations\n", n_operations);
    free_list(l1);
    free_list(l2);
    free_list(piv_list);

    return 0;
}
