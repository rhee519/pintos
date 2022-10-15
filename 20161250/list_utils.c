#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "list_utils.h"

struct list_item *list_new_item(int value)
{
    struct list_item *it = malloc(sizeof(struct list_item));
    it->data = value;
    return it;
}

void list_dumpdata(struct list *list)
{
    if (list == NULL || list_size(list) == 0)
        return;

    struct list_elem *e = list_head(list);
    while ((e = list_next(e)) != list_end(list))
    {
        struct list_item *it = list_entry(e, struct list_item, elem);
        printf("%d ", it->data);
    }
    printf("\n");
}

void list_push_front_by_value(struct list *list, int value)
{
    struct list_item *it = list_new_item(value);
    list_push_front(list, &it->elem);
}

void list_push_back_by_value(struct list *list, int value)
{
    struct list_item *it = list_new_item(value);
    list_push_back(list, &it->elem);
}

void list_insert_by_index_and_value(struct list *list, int idx, int value)
{
    size_t size = list_size(list);
    if (idx == size)
    {
        list_push_back_by_value(list, value);
    }
    else
    {
        struct list_elem *before = list_at(list, idx);
        struct list_item *it = list_new_item(value);
        list_insert(before, &it->elem);
    }
}

void list_insert_ordered_by_value(struct list *list, int value)
{
    struct list_item *it = list_new_item(value);
    list_insert_ordered(list, &it->elem, list_less, NULL);
}

/*
  list의 모든 노드 위치를 임의로 shuffle
*/
void list_shuffle(struct list *list)
{
    srand(time(NULL));
    size_t size = list_size(list);
    for (int i = 0; i < size; i++)
    {
        int idx1 = rand() % size, idx2 = rand() % size;
        list_swap(list_at(list, idx1), list_at(list, idx2));
    }
}

/*
  delete list 시 매모리 해제를 위한 destructor
*/
void list_destroy(struct list *list)
{
    while (!list_empty(list))
    {
        list_pop_back(list);
    }
}
