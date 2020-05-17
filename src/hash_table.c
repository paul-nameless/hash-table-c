#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "prime.h"
#include "hash_table.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};
static int HT_PRIME_1 = 151;
static int HT_PRIME_2 = 163;
static int HT_INITIAL_BASE_SIZE = 53;

static void ht_resize_up(ht_hash_table* ht);
static void ht_resize_down(ht_hash_table* ht);

static ht_item* ht_new_item(const char* k, const char* v) {
  ht_item* i = malloc(sizeof(ht_item));
  i->key = strdup(k);
  i->value = strdup(v);
  return i;
}

static ht_hash_table* ht_new_sized(const int base_size) {
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));
  ht->base_size = base_size;
  ht->size = next_prime(ht->base_size);
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  return ht;
}

ht_hash_table* ht_new() {
  return ht_new_sized(HT_INITIAL_BASE_SIZE);
}


static void ht_del_item(ht_item* i) {
  free(i->key);
  free(i->value);
  free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

/* static int hash(const char* str, int prime, num_buckets) { */
static int ht_hash(const char* str, const int prime, const int num_buckets) {
  long hash = 0;
  const int len = strlen(str);
  for (int i = 0; i < len; i++) {
    hash += (long)pow(prime, len - (i + 1)) * str[i];
    hash = hash % num_buckets;
  }
  return (int)hash;
}

static int ht_get_hash(const char* s,
                       const int num_buckets,
                       const int attempt
                       ) {
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_set(ht_hash_table* ht, const char* key, const char* value) {
  const int load = ht->count * 100 / ht->size;
  if (load > 70) {
    ht_resize_up(ht);
  }
  ht_item* item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item* cur_item = ht->items[index];
  int i = 1;
  while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
    if (cur_item != &HT_DELETED_ITEM) {
      if (strcmp(cur_item->key, key) == 0) {
        ht_del_item(cur_item);
        ht->items[index] = item;
        return;
      }
    }
    index = ht_get_hash(item->key, ht->size, i);
    cur_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  ht->count++;
}

char* ht_get(ht_hash_table* ht, const char*key) {
  int index = ht_get_hash(key, ht->size, 0);
  ht_item* item = ht->items[index];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        return item->value;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  return NULL;
}

void ht_del(ht_hash_table* ht, const char*key) {
  const int load = ht->count * 100 / ht->size;
  if (load < 10) {
    ht_resize_down(ht);
  }
  int index = ht_get_hash(key, ht->size, 0);
  ht_item* item = ht->items[index];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        ht_del_item(item);
        ht->items[index] = &HT_DELETED_ITEM;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  ht->count--;
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
  if (base_size < HT_INITIAL_BASE_SIZE) {
    return;
  }
  ht_hash_table* tmp_ht = ht_new_sized(base_size);
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_set(tmp_ht, item->key, item->value);
    }
  }

  ht->base_size = tmp_ht->base_size;
  ht->count = tmp_ht->count;

  const int tmp_size = ht->size;
  ht->size = tmp_ht->size;
  tmp_ht->size = tmp_size;

  ht_item** tmp_items = ht->items;
  ht->items = tmp_ht->items;
  tmp_ht->items = tmp_items;

  ht_del_hash_table(tmp_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
  const int new_size = ht->base_size * 2;
  ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht) {
  const int new_size = ht->base_size / 2;
  ht_resize(ht, new_size);
}
