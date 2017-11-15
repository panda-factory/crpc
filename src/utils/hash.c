#include <stdio.h>
#include <stdlib.h>


#include "log.h"
#include "hash.h"
#include "error.h"
#include "define.h"

hash_func_t g_hash_func = jenkins_hash_func;

#if DESC("内部函数")
/**
 * @brief alloc memory for hash node, and return point
 *
 * @param table: hash node belong to
 * @param key: hash key point
 * @param key_len: hash key length, terminator('\0') can not to be containned.
 * @param val: value for hash key in hash table
 * @param val_len: value length
 *
 * @return hash node point
 */
static hash_node_t *
hash_new_node(hash_table_t *table, const char *key, const void *val, const uint32_t val_len)
{
    uint32_t key_len;
    uint32_t keyval_len;
    uint32_t memory_size;
    hash_node_t *hash_node;
    tlv_t *tlv;

    CHECK_NULL_RETURN_NULL(table, "cannot receive table = (null).");

    /* memory size of hash-node (size of hash-node head + two size of TLV) */
    key_len = strlen(key) + 1;
    keyval_len = 2*sizeof(tlv_t) + key_len + val_len;
    memory_size = sizeof(hash_node_t) + keyval_len;
    hash_node = (hash_node_t *)calloc(1u, memory_size);
    CHECK_NULL_RETURN_NULL(hash_node, "calloc memory failed.");

    hash_node->table = table;
    hash_node->length = keyval_len;

    /* memcpy for tlv-key */
    tlv = (tlv_t *)hash_keyval(hash_node);
    tlv_init(tlv, TLV_TYPE_HASH_KEY, key_len, key);

    /* memcpy for TLV-val */
    tlv = (tlv_t *)tlv_next(tlv);
    tlv_init(tlv, TLV_TYPE_HASH_VALUE, val_len, val);

    return hash_node;
}

static uint32_t
jenkins_hash_func(const char *key, uint16_t key_len)
{
     uint32_t hash_x, hash_y, hash_z, hash_val;
     hash_val = 0xfeedbeefu;
     hash_x = hash_y = 0x9e3779b9u;
     hash_z = key_len;
     while (hash_z >= 12U) {
         hash_x += (key[0] + ((uint32_t)key[1] << 8) + ((uint32_t)key[2] << 16) + ((uint32_t)key[3] << 24));
         hash_y += (key[4] + ((uint32_t)key[5] << 8) + ((uint32_t)key[6] << 16) + ((uint32_t)key[7] << 24)); 
         hash_val += (key[8] + ((uint32_t)key[9] << 8) + ((uint32_t)key[10] << 16) + ((uint32_t)key[11] << 24));
        jenkins_hash_mix(&hash_x, &hash_y, &hash_val);
        key += 12;
        hash_z -= 12U;
     }
     hash_val += (uint16_t)(key_len);
     switch (hash_z) {
         case 11: hash_val += ((uint32_t)key[10] << 24);
         case 10: hash_val += ((uint32_t)key[9] << 16);
         case 9:  hash_val += ((uint32_t)key[8] << 8);
         case 8:  hash_y += ((uint32_t)key[7] << 24);
         case 7:  hash_y += ((uint32_t)key[6] << 16);
         case 6:  hash_y += ((uint32_t)key[5] << 8);
         case 5:  hash_y += key[4];
         case 4:  hash_x += ((uint32_t)key[3] << 24);
         case 3:  hash_x += ((uint32_t)key[2] << 16);
         case 2:  hash_x += ((uint32_t)key[1] << 8);
         case 1:  hash_x += key[0];
     }
     jenkins_hash_mix(&hash_x, &hash_y, &hash_val);

     return hash_val;
}

static void
jenkins_hash_mix(uint32_t *hash_x, uint32_t *hash_y, uint32_t *hash_val)
{
    uint32_t a = *hash_x;
    uint32_t b = *hash_y;
    uint32_t c = *hash_val;

    a -= b; a -= c; a ^= (c >> 13);
    b -= c; b -= a; b ^= (a << 8);
    c -= a; c -= b; c ^= (b >> 13);
    a -= b; a -= c; a ^= (c >> 12);
    b -= c; b -= a; b ^= (a << 16);
    c -= a; c -= b; c ^= (b >> 5);
    a -= b; a -= c; a ^= (c >> 3); 
    b -= c; b -= a; b ^= (a << 10);
    c -= a; c -= b; c ^= (b >> 15); 

    *hash_x = a;
    *hash_y = b;
    *hash_val = c;
}
#endif //内部函数

tlv_t *
hash_keyval(const hash_node_t *ptr)
{
    tlv_t *tlv;
    CHECK_NULL_RETURN_NULL(ptr, "cannot receive a hash-node = (null).");

    tlv = (tlv_t *)&ptr->keyval;
    return tlv;
}
/**
 * @brief alloc memory for hash table, and return point
 * @param num_buckets: buckets number in hash table
 * @return hash table point
 */
hash_table_t *
hash_new(const char *name, uint32_t num_buckets)
{
    uint32_t mem_size;
    hash_table_t *table;

    CHECK_NULL_RETURN_NULL(name, "cannot receive a name(null)");

    /* memory size of hash-table (size of hash-table head + size of buckets-array */
    mem_size = sizeof(char *) + sizeof(uint32_t) + num_buckets*sizeof(list_table_t);

    table = (hash_table_t *)calloc(1, mem_size);
    CHECK_NULL_RETURN_NULL(table, "calloc memory failed.");

    table->name = strdup(name);
    CHECK_NULL_RETURN_NULL(table->name, "strdup name memory failed.");
    table->num_buckets = num_buckets;
    table->num_datas = 0;

    return table;
}

/**
 * @brief push hash-node into hash-table
 *
 * @param table: hash node belong to 
 * @param key: hash key poing
 * @param key_len: hash-key length,  terminator('\0') can not to be containned.
 * @param val: value for hash key in hash table
 * @param val_len: value length
 *
 * @return success for OK, otherwise for <ERROR_CODE>
 */
int
hash_add(hash_table_t *table, const char *key, const void *val, const uint32_t val_len)
{
    uint32_t hash_val;
    hash_bucket_t *bucket = NULL;
    hash_node_t *hash_node = NULL;

    CHECK_NULL_RETURN_ERROR(table, "cannot receive table = (null).");

    /*申请哈希节点.*/
    hash_node = hash_new_node(table, key, val, val_len);
    CHECK_NULL_RETURN_ERROR(hash_node, "new hash node failed.");

    hash_val = g_hash_func(key, strlen(key) + 1) % table->num_buckets;
    bucket = table->buckets + hash_val;

    if (NULL != hash_lookup(table, key)) {
        ERROR_LOG("hash node with key: %s is exist.", key);
        return ERROR;
    }

    list_append(&bucket->list, &hash_node->list_node);
    table->num_datas++;
    bucket->num++;

    return OK;
}

/**
 * @brief lookup hash-key in hash-bucket, and output hash-node.
 *
 * @param bucket: hash-bucket.
 * @param key: hash-key for lookup.
 * @param node <output>: hash-node to output.
 *
 * @return hash-node.
 */
void *
hash_get(const hash_table_t *table, const char *key)
{
    hash_node_t *node = NULL;
    tlv_t *tlv = NULL;

    CHECK_2_NULL_RETURN_NULL(table, key, "input params error.");

    node = hash_lookup(table, key);
    CHECK_NULL_RETURN_NULL(node, "hash with key: %s is not exist.", key);

    tlv = hash_keyval(node);
    tlv = tlv_next(tlv);

    return tlv_value(tlv);
}

/**
 * @brief lookup hash-key in hash-bucket, and output hash-node.
 *
 * @param bucket: hash-bucket.
 * @param key: hash-key for lookup.
 * @param node <output>: hash-node to output.
 *
 * @return hash-node.
 */
hash_node_t *
hash_lookup(const hash_table_t *hash_table, const char *hash_key)
{
    uint32_t hash_value;
    uint32_t key_length;
    const hash_bucket_t *hash_bucket;
    list_node_t *iter;
    hash_node_t *tmp_node;

    CHECK_2_NULL_RETURN_NULL(hash_table, hash_key, "cannot receive params = (null).");

    key_length = strlen(hash_key) + 1;
    hash_value = g_hash_func(hash_key, key_length) % hash_table->num_buckets;
    hash_bucket = hash_table->buckets + hash_value;

    iter = hash_bucket->list.head;
    while (NULL != iter) {
        tmp_node = LIST_ENTRY(iter, hash_node_t, list_node);
        if (0 == strcmp(hash_key, tmp_node->keyval->value)) { 
            return tmp_node;
        }
        iter = iter->next;
    }

    return NULL;
}

