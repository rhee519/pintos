#include "list.h"

#define LIST_ITEM(X) list_entry((X), struct list_item, elem)

#define LIST_FRONT "list_front"
#define LIST_BACK "list_back"
#define LIST_INSERT "list_insert"
#define LIST_INSERT_ORDERED "list_insert_ordered"
#define LIST_PUSH_FRONT "list_push_front"
#define LIST_PUSH_BACK "list_push_back"
#define LIST_POP_FRONT "list_pop_front"
#define LIST_POP_BACK "list_pop_back"
#define LIST_EMPTY "list_empty"
#define LIST_SIZE "list_size"
#define LIST_REVERSE "list_reverse"
#define LIST_SWAP "list_swap"
#define LIST_SHUFFLE "list_shuffle"
#define LIST_SORT "list_sort"
#define LIST_SPLICE "list_splice"
#define LIST_UNIQUE "list_unique"
#define LIST_MAX "list_max"
#define LIST_MIN "list_min"
#define LIST_REMOVE "list_remove"

struct list_item *list_new_item(int);
void list_dumpdata(struct list *);
void list_push_front_by_value(struct list *list, int);
void list_push_back_by_value(struct list *list, int);
void list_insert_by_index_and_value(struct list *list, int idx, int);
void list_insert_ordered_by_value(struct list *list, int);
void list_shuffle(struct list *list);
void list_destroy(struct list *list);
