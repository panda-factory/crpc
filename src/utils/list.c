#include <stdlib.h>

#include "log.h"
#include "list.h"
#include "error.h"
#include "define.h"

list_table_t *
list_new()
{
    list_table_t *table = NULL;

    table = (list_table_t *)malloc(sizeof(list_table_t));
    CHECK_NULL_RETURN_NULL(table, "malloc list-table failure.");

    table->head = NULL;
    table->tail = NULL;

    return table;
}

int 
list_append(list_table_t *table, list_node_t *node)
{
    CHECK_2_NULL_RETURN_ERROR(table, node, "input params table || node == NULL.");

    if (NULL == table->head) {
        node->prev = NULL;
        table->head = node;
    } else {
        node->prev = table->tail;
        table->tail->next = node;
    }
    table->tail = node;
    node->next = NULL;

    return OK;
}

int
list_insert(list_node_t *node_dst, list_node_t *node_src)
{
    CHECK_2_NULL_RETURN_ERROR(node_dst, node_src, "input params node_dst || node_src == NULL.");

    node_dst->prev->next = node_src;
    node_src->prev = node_dst->prev;
    node_src->next = node_dst;
    node_dst->prev = node_src;

    return OK;
}

int 
list_remove(list_node_t *node)
{
    CHECK_NULL_RETURN_ERROR(node, "input param node == NULL.");

    node->prev->next = node->next;
    node->next->prev = node->prev;

    return OK;
}

void
list_free(list_table_t *table, free_func_t free_func)
{
    list_node_t *iter = NULL;
    list_node_t *next = NULL;

    CHECK_NULL_RETURN_VOID(table, "input param table == NULL.");

    iter = table->head;
    while (NULL != iter) {
        next = iter->next;
        free_func(iter);
        iter = next;
    }
}




