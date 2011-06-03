//////////////////////////////////////////////////////////////
// queue.h
//     reference of queue functions
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

void add_node_to(void **head, void **tail, void *node, void *to, int before_or_after);
void move_node(void **head, void **tail, void *moved, void *to, int before_or_after);
void add_node(void **head, void **tail, void *node);
void del_node(void **head, void **tail, void *node);
typedef int (*CMP_FUNC)(void *t1, void *t2);
void sort_list(void **head, void **tail, CMP_FUNC nodcmp);
void add_node_sorted(void **head, void **tail, void *node, CMP_FUNC nodcmp);

#ifdef __cplusplus
};
#endif