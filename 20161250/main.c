#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "list_utils.h"
#include "hash.h"
#include "hash_utils.h"
#include "bitmap.h"
#include "bitmap_utils.h"

/*
   TYPES
 */
#define LIST "list"
#define HASHTABLE "hashtable"
#define HASH "hash"
#define BITMAP "bitmap"
#define BM "bm"

/*
   COMMON MACRO KEYWORDS
 */
#define QUIT "quit"
#define CREATE "create"
#define DELETE "delete"
#define DUMPDATA "dumpdata"
#define TRUE "true"
#define FALSE "false"

/*
   COMMON UTILS
 */
int parse_index(char *str)
{
	/*
	   "list1" => 1
	   "hash7" => 7
	 */
	int len = strlen(str);
	int i = 0;
	while (i < len && (str[i] < '0' || str[i] > '9'))
		i++;
	if (i == len)
		return -1;
	return atoi(str + i);
}

static inline bool is(char *str1, char *str2)
{
	return strcmp(str1, str2) == 0;
}

static inline bool contains(char *str1, char *str2)
{
	return strncmp(str1, str2, strlen(str2)) == 0;
}

/*
   Test program
 */
int main(int argc, char *argv[])
{
	struct list list[10];
	struct hash hash[10];
	struct bitmap *bitmap[10]; // struct bitmap is abstract in bitmap.h

	char command[100], type[20], name[20];
	int args[5];

	// execute commands while command != "quit"
	while (scanf("%s", command) != EOF)
	{
		if (is(command, QUIT))
			break;
		else if (is(command, CREATE))
		{
			scanf("%s %s", type, name);
			int id = parse_index(name);
			if (is(type, LIST))
			{
				list_init(&list[id]);
			}
			else if (is(type, HASHTABLE))
			{
				hash_init(&hash[id], hash_int_func, hash_less_int_func, NULL);
			}
			else if (is(type, BITMAP))
			{
				scanf("%d", &args[0]);
				bitmap[id] = bitmap_create((size_t)args[0]);
				// printf("%p\n", bitmap[id]);
			}
		}
		else if (is(command, DELETE))
		{
			scanf("%s", name);
			int id = parse_index(name);
			if (contains(name, LIST))
			{
				list_destroy(&list[id]);
			}
			else if (contains(name, HASH))
			{
				hash_destroy(&hash[id], hash_destructor);
			}
			else if (contains(name, BM))
			{
				bitmap_destroy(bitmap[id]);
			}
		}
		else if (is(command, DUMPDATA))
		{
			scanf("%s", name);
			int id = parse_index(name);
			if (contains(name, LIST))
			{
				list_dumpdata(&list[id]);
			}
			else if (contains(name, HASH))
			{
				hash_dumpdata(&hash[id]);
			}
			else if (contains(name, BM))
			{
				bitmap_dumpdata(bitmap[id]);
			}
		}
		//
		// LIST
		//
		else if (is(command, LIST_FRONT))
		{
			scanf("%s", name);
			int id = parse_index(name);
			// struct list_item *it = get_list_item(list_front(&list[id]));
			// struct list_item *it = list_entry(list_front(&list[id]), struct list_item, elem);
			struct list_item *it = LIST_ITEM(list_front(&list[id]));
			printf("%d\n", it->data);
		}
		else if (is(command, LIST_BACK))
		{
			scanf("%s", name);
			int id = parse_index(name);
			// struct list_item *it = get_list_item(list_back(&list[id]));
			struct list_item *it = LIST_ITEM(list_back(&list[id]));
			printf("%d\n", it->data);
		}
		else if (is(command, LIST_PUSH_FRONT))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			list_push_front_by_value(&list[id], args[0]);
		}
		else if (is(command, LIST_PUSH_BACK))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			list_push_back_by_value(&list[id], args[0]);
		}
		else if (is(command, LIST_POP_FRONT))
		{
			scanf("%s", name);
			int id = parse_index(name);
			list_pop_front(&list[id]);
		}
		else if (is(command, LIST_POP_BACK))
		{
			scanf("%s", name);
			int id = parse_index(name);
			list_pop_back(&list[id]);
		}
		else if (is(command, LIST_INSERT))
		{
			scanf("%s %d %d", name, &args[0], &args[1]);
			int id = parse_index(name);
			list_insert_by_index_and_value(&list[id], args[0], args[1]);
		}
		else if (is(command, LIST_INSERT_ORDERED))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			list_insert_ordered_by_value(&list[id], args[0]);
		}
		else if (is(command, LIST_EMPTY))
		{
			scanf("%s", name);
			int id = parse_index(name);
			bool is_empty = list_empty(&list[id]);
			printf("%s\n", is_empty ? TRUE : FALSE);
		}
		else if (is(command, LIST_MAX))
		{
			scanf("%s", name);
			int id = parse_index(name);
			struct list_item *it_max = get_list_item(list_max(&list[id], list_less, NULL));
			printf("%d\n", it_max->data);
		}
		else if (is(command, LIST_MIN))
		{
			scanf("%s", name);
			int id = parse_index(name);
			struct list_item *it_min = get_list_item(list_min(&list[id], list_less, NULL));
			printf("%d\n", it_min->data);
		}
		else if (is(command, LIST_SIZE))
		{
			scanf("%s", name);
			int id = parse_index(name);
			size_t size = list_size(&list[id]);
			printf("%zu\n", size);
		}
		else if (is(command, LIST_REMOVE))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			struct list_elem *elem_delete = list_at(&list[id], args[0]);
			list_remove(elem_delete);
		}
		else if (is(command, LIST_REVERSE))
		{
			scanf("%s", name);
			int id = parse_index(name);
			list_reverse(&list[id]);
		}
		else if (is(command, LIST_SHUFFLE))
		{
			scanf("%s", name);
			int id = parse_index(name);
			list_shuffle(&list[id]);
		}
		else if (is(command, LIST_SORT))
		{
			scanf("%s", name);
			int id = parse_index(name);
			list_sort(&list[id], list_less, NULL);
		}
		else if (is(command, LIST_SPLICE))
		{
			char name1[20];
			scanf("%s %d %s %d %d", name, &args[0], name1, &args[1], &args[2]);
			int id0 = parse_index(name), id1 = parse_index(name1);
			list_splice(list_at(&list[id0], args[0]),
						list_at(&list[id1], args[1]),
						list_at(&list[id1], args[2]));
		}
		else if (is(command, LIST_SWAP))
		{
			scanf("%s %d %d", name, &args[0], &args[1]);
			int id = parse_index(name);
			list_swap(list_at(&list[id], args[0]), list_at(&list[id], args[1]));
		}
		else if (is(command, LIST_UNIQUE))
		{
			char buf[100], name1[20];
			fgets(buf, 100, stdin);
			sscanf(buf, "%s %s", name, name1);
			int id0 = parse_index(name);
			if (strlen(name1) > 0)
			{
				int id1 = parse_index(name1);
				list_unique(&list[id0], &list[id1], list_less, NULL);
				name1[0] = '\0'; // clear name1
			}
			else
			{
				list_unique(&list[id0], NULL, list_less, NULL);
			}
		}
		//
		// HASH
		//
		else if (is(command, HASH_INSERT))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			struct hash_item *item = hash_new_item(args[0]);

			hash_insert(&hash[id], &item->elem);
		}
		else if (is(command, HASH_APPLY))
		{
			char apply_func[20];
			scanf("%s %s", name, apply_func);
			int id = parse_index(name);
			if (is(apply_func, "square"))
			{
				hash_apply(&hash[id], hash_square);
			}
			else if (is(apply_func, "triple"))
			{
				hash_apply(&hash[id], hash_cube);
			}
		}
		else if (is(command, HASH_DELETE))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			struct hash_item item;
			item.data = args[0];
			hash_delete(&hash[id], &item.elem);
		}
		else if (is(command, HASH_FIND))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			struct hash_item item;
			item.data = args[0];
			struct hash_elem *e_found = hash_find(&hash[id], &item.elem);
			if (e_found)
			{
				struct hash_item *it_found = get_hash_item(e_found);
				printf("%d\n", it_found->data);
			}
		}
		else if (is(command, HASH_REPLACE))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			struct hash_item *item = hash_new_item(args[0]);
			// struct hash_elem *old = hash_replace(&hash[id], &item->elem);
			hash_replace(&hash[id], &item->elem);
		}
		else if (is(command, HASH_EMPTY))
		{
			scanf("%s", name);
			int id = parse_index(name);
			bool is_empty = hash_empty(&hash[id]);
			printf("%s\n", is_empty ? TRUE : FALSE);
		}
		else if (is(command, HASH_SIZE))
		{
			scanf("%s", name);
			int id = parse_index(name);
			size_t size = hash_size(&hash[id]);
			printf("%zu\n", size);
		}
		else if (is(command, HASH_CLEAR))
		{
			scanf("%s", name);
			int id = parse_index(name);
			hash_clear(&hash[id], NULL);
		}
		//
		// BITMAP
		//
		else if (is(command, BITMAP_MARK))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			bitmap_mark(bitmap[id], (size_t)args[0]);
		}
		else if (is(command, BITMAP_FLIP))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			bitmap_flip(bitmap[id], args[0]);
		}
		else if (is(command, BITMAP_COUNT))
		{
			char boolstr[10];
			scanf("%s %d %d %s", name, &args[0], &args[1], boolstr);
			int id = parse_index(name);
			size_t cnt = bitmap_count(bitmap[id], args[0], args[1], is(boolstr, TRUE));
			printf("%zu\n", cnt);
		}
		else if (is(command, BITMAP_CONTAINS))
		{
			char boolstr[10];
			scanf("%s %d %d %s", name, &args[0], &args[1], boolstr);
			int id = parse_index(name);
			bool if_contains = bitmap_contains(bitmap[id], args[0], args[1], is(boolstr, TRUE));
			printf("%s\n", if_contains ? TRUE : FALSE);
		}
		else if (is(command, BITMAP_SET))
		{
			char boolstr[10];
			scanf("%s %d %s", name, &args[0], boolstr);
			int id = parse_index(name);
			bitmap_set(bitmap[id], args[0], is(boolstr, TRUE));
		}
		else if (is(command, BITMAP_SET_ALL))
		{
			char boolstr[10];
			scanf("%s %s", name, boolstr);
			int id = parse_index(name);
			bitmap_set_all(bitmap[id], is(boolstr, TRUE));
		}
		else if (is(command, BITMAP_SET_MULTIPLE))
		{
			char boolstr[10];
			scanf("%s %d %d %s", name, &args[0], &args[1], boolstr);
			int id = parse_index(name);
			bitmap_set_multiple(bitmap[id], args[0], args[1], is(boolstr, TRUE));
		}
		else if (is(command, BITMAP_SIZE))
		{
			scanf("%s", name);
			int id = parse_index(name);
			size_t size = bitmap_size(bitmap[id]);
			printf("%zu\n", size);
		}
		else if (is(command, BITMAP_SCAN))
		{
			char boolstr[10];
			scanf("%s %d %d %s", name, &args[0], &args[1], boolstr);
			int id = parse_index(name);
			size_t cnt_scan = bitmap_scan(bitmap[id], args[0], args[1], is(boolstr, TRUE));
			printf("%zu\n", cnt_scan);
		}
		else if (is(command, BITMAP_SCAN_AND_FLIP))
		{
			char boolstr[10];
			scanf("%s %d %d %s", name, &args[0], &args[1], boolstr);
			int id = parse_index(name);
			size_t cnt_scan = bitmap_scan_and_flip(bitmap[id], args[0], args[1], is(boolstr, TRUE));
			printf("%zu\n", cnt_scan);
		}
		else if (is(command, BITMAP_RESET))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			bitmap_reset(bitmap[id], args[0]);
		}
		else if (is(command, BITMAP_NONE))
		{
			scanf("%s %d %d", name, &args[0], &args[1]);
			int id = parse_index(name);
			bool is_none = bitmap_none(bitmap[id], args[0], args[1]);
			printf("%s\n", is_none ? TRUE : FALSE);
		}
		else if (is(command, BITMAP_DUMP))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			bitmap_dump(bitmap[id]);
		}
		else if (is(command, BITMAP_ALL))
		{
			scanf("%s %d %d", name, &args[0], &args[1]);
			int id = parse_index(name);
			bool is_all = bitmap_all(bitmap[id], args[0], args[1]);
			printf("%s\n", is_all ? TRUE : FALSE);
		}
		else if (is(command, BITMAP_ANY))
		{
			scanf("%s %d %d", name, &args[0], &args[1]);
			int id = parse_index(name);
			bool is_any = bitmap_any(bitmap[id], args[0], args[1]);
			printf("%s\n", is_any ? TRUE : FALSE);
		}
		else if (is(command, BITMAP_TEST))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			bool test_result = bitmap_test(bitmap[id], args[0]);
			printf("%s\n", test_result ? TRUE : FALSE);
		}
		else if (is(command, BITMAP_EXPAND))
		{
			scanf("%s %d", name, &args[0]);
			int id = parse_index(name);
			bitmap[id] = bitmap_expand(bitmap[id], args[0]);
		}
	}

	return 0;
}
