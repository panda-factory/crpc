#ifndef _LIST_H_
#define _LIST_H_

#include "log.h"
/*--------------------------------------------------------------
 * get from linux kernel list.h.
 */
#define OFFSET_OF(type, member) ((int64_t)&((type *)0)->member)
#define CONTAINER_OF(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - OFFSET_OF(type, member) );})
#define LIST_ENTRY(ptr, type, member) \
    CONTAINER_OF(ptr, type, member)

typedef struct _list_node_t {
    struct _list_node_t *prev;
    struct _list_node_t* next;
} list_node_t;
typedef struct _list_table_t {
    list_node_t *head;
    list_node_t *tail;
} list_table_t;

typedef void (*free_func_t)(void *ptr);
/*------------------------------------------------------------*/
list_table_t *
list_new();

int 
list_append(list_table_t *table, list_node_t *node);

int 
list_insert(list_node_t *node_dst, list_node_t *node_src);
    
int 
list_remove(list_node_t *node);

void
list_free(list_table_t *table, free_func_t free_func);

#endif //_LIST_H
