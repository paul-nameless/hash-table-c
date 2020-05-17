typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    int base_size;
    ht_item** items;
} ht_hash_table;


ht_hash_table* ht_new();

void ht_del_hash_table(ht_hash_table* ht);

void ht_set(ht_hash_table* ht, const char* key, const char* value);

char* ht_get(ht_hash_table* ht, const char* key);

void ht_del(ht_hash_table* h, const char* key);

/* int hash(const char* str, const int prime, const int num_buckets); */
