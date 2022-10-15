#include "hash.h"

#define HASH_INSERT "hash_insert"
#define HASH_APPLY "hash_apply"
#define HASH_DELETE "hash_delete"
#define HASH_EMPTY "hash_empty"
#define HASH_SIZE "hash_size"
#define HASH_CLEAR "hash_clear"
#define HASH_FIND "hash_find"
#define HASH_REPLACE "hash_replace"

struct hash_item *hash_new_item(int value);
void hash_dumpdata(struct hash *hash);

hash_action_func hash_square, hash_cube;
void hash_square(struct hash_elem *, void *aux);
void hash_cube(struct hash_elem *, void *aux);