#include <stdio.h>

#include "hash_table.h"

int main() {
  ht_hash_table* ht = ht_new();
  for (int i = 0; i < 100; i++) {
    char key[10];
    char value[11];
    /* printf("setting: %d\n", i); */
    sprintf(key, "key-%d", i);
    sprintf(value, "value-%d", i);
    ht_set(ht, key, value);
  }
  printf("%s\n", ht_get(ht, "key-1"));
  printf("%s\n", ht_get(ht, "key-99"));
  ht_del(ht, "key-99");
  printf("%s\n", ht_get(ht, "key-99"));
  printf("%s\n", ht_get(ht, "key-100"));
  ht_del_hash_table(ht);
  return 0;
}
