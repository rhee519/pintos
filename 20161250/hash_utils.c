#include <stdio.h>
#include <stdlib.h>

#include "list_utils.h"
#include "hash_utils.h"

/*
    int value값을 가지는 hash_elem을 동적으로 생성하여 포인터를 return
*/
struct hash_item *hash_new_item(int value)
{
    struct hash_item *it = malloc(sizeof(struct hash_item));
    it->data = value;
    return it;
}

/*
    hash_iterator를 이용하여 hash의 모든 데이터를 print
*/
void hash_dumpdata(struct hash *hash)
{
    if (hash == NULL || hash_size(hash) == 0)
        return;

    struct hash_iterator it;
    hash_first(&it, hash);
    while (hash_next(&it))
    {
        struct hash_item *h_item = get_hash_item(hash_cur(&it));
        printf("%d ", h_item->data);
    }
    printf("\n");
}

/*
    hash의 각 elem 값을 제곱(square)
    key index는 별도로 수정하지 않는다.
*/
void hash_square(struct hash_elem *e, void *aux)
{
    struct hash_item *item = get_hash_item(e);
    item->data = item->data * item->data;
}

/*
    hash의 각 elem 값을 세제곱(cube)
    key index는 별도로 수정하지 않는다.
*/
void hash_cube(struct hash_elem *e, void *aux)
{
    struct hash_item *item = get_hash_item(e);
    item->data = item->data * item->data * item->data;
}