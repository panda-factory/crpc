#ifndef _HASH_H_
#define _HASH_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "tlv.h"

#define HASH_SEED   0XFEEDBEEFU

typedef struct _hash_bucket_t {
    uint32_t        num;
    list_table_t    list;
} hash_bucket_t;

typedef struct _hash_table_t {
    char            *name;
    uint32_t        num_buckets;  // buckets number
    uint32_t        num_datas;
    hash_bucket_t   buckets[];    // head of bucker-array
} hash_table_t;

typedef struct _hash_node_t {
    hash_table_t  *table;    // which hash-table is hash-node belong to
    list_node_t   list_node; // list for hash-bucket reference, and handle conflict.
    uint32_t      length;    // memory size for key_val
    tlv_t         keyval[];  // contain two tlv items (tlv-key + tlv-val)
} hash_node_t;

typedef uint32_t (*hash_func_t)(const char *key, const uint16_t key_len);

tlv_t *
hash_keyval(const hash_node_t *ptr);

hash_table_t *
hash_new(const char *name, uint32_t num_buckets);

int
hash_add(hash_table_t *table, const char *key, const void *val, const uint32_t val_len);

void *
hash_get(const hash_table_t *table, const char *key);

hash_node_t *
hash_lookup(const hash_table_t *table, const char *key);

int
destroy_hash_table(hash_table_t *table);

static uint32_t
jenkins_hash_func(const char *key, uint16_t key_len);
static void
jenkins_hash_mix(uint32_t *hash_x, uint32_t *hash_y, uint32_t *hash_val);
#endif //_HASH_H_
