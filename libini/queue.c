#include <stdlib.h>
#include "queue.h"

struct basic_nod_t {
	struct basic_nod_t *prev;
	struct basic_nod_t *next;
};

void add_node_to(void **head, void **tail, void *node, void *to, int before_or_after)
{
	struct basic_nod_t **h = (struct basic_nod_t**)head;
	struct basic_nod_t **t = (struct basic_nod_t**)tail;
	struct basic_nod_t *n = (struct basic_nod_t*)node;
	struct basic_nod_t *tt = (struct basic_nod_t*)to;
	if(*h == NULL)
	{
		*h = *t = n;
		n->prev = NULL;
		n->next = NULL;
	}
	else if(before_or_after == 0)
	{
		//move the 'moved' node to the place before 'to'
		if(tt == NULL)
			tt = *h;
		if(*h == tt)
			*h = n;
		n->prev = tt->prev;
		n->next = tt;
		tt->prev = n;
		if(n->prev != NULL)
		{
			n->prev->next = n;
		}
	}
	else if(before_or_after == 1)
	{
		//move the 'moved' node to the place after 'to'
		if(tt == NULL)
			tt = *t;
		if(*t == tt)
			*t = n;
		n->next = tt->next;
		n->prev = tt;
		tt->next = n;
		if(n->next != NULL)
			n->next->prev = n;
	}
}

void add_node(void **head, void **tail, void *node)
{
	add_node_to(head, tail, node, NULL, 1);
}

void del_node(void **head, void **tail, void *node)
{
	struct basic_nod_t **h = (struct basic_nod_t**)head;
	struct basic_nod_t **t = (struct basic_nod_t**)tail;
	struct basic_nod_t *n = (struct basic_nod_t*)node;
	if(n == *h)
	{
		*h = n->next;
	}
	if(n == *t)
	{
		*t = n->prev;
	}
	if(n->prev != NULL)
		n->prev->next = n->next;
	if(n->next != NULL)
		n->next->prev = n->prev;
}

void move_node(void **head, void **tail, void *moved, void *to, int before_or_after)
{
	struct basic_nod_t **h = (struct basic_nod_t**)head;
	struct basic_nod_t **t = (struct basic_nod_t**)tail;
	struct basic_nod_t *m = (struct basic_nod_t*)moved;
	struct basic_nod_t *tt = (struct basic_nod_t*)to;
	if(	(h == NULL) || (t == NULL)
		|| (*h == NULL) || (*t == NULL)
		|| (m == tt))
		return;
	del_node(head, tail, moved);
	add_node_to(head, tail, moved, to, before_or_after);
}

void sort_list(void **head, void **tail, CMP_FUNC nodcmp)
{
	struct basic_nod_t **h = (struct basic_nod_t**)head;
	struct basic_nod_t *nod1 = *h;
	struct basic_nod_t *nod2 = nod1->next;
	int swaped = 1;
	while(swaped)
	{
		swaped = 0;
		while(1)
		{
			if((*nodcmp)(nod1, nod2) > 0)
			{
				move_node(head, tail, nod1, nod2, 1);
				nod2 = nod1->next;
				swaped = 1;
			}
			else
			{
				nod1 = nod2;
				nod2 = nod2->next;
			}
			if(nod2 == NULL)
			{
				nod1 = *head;
				nod2 = nod1->next;
				break;
			}
		}
	}
}

void add_node_sorted(void **head, void **tail, void *node, CMP_FUNC nodcmp)
{
	struct basic_nod_t **h = (struct basic_nod_t**)head;
	struct basic_nod_t **t = (struct basic_nod_t**)tail;
	struct basic_nod_t *n = (struct basic_nod_t*)node;
	struct basic_nod_t *tmp = *h;
	if((*nodcmp)(n, *h) < 0)
	{
		add_node_to(head, tail, node, *h, 0);
		return;
	}
	if((*nodcmp)(n, *t) >= 0)
	{
		add_node(head, tail, node);
		return;
	}
	while(tmp)
	{
		if((*nodcmp)(n, tmp) < 0)
		{
			add_node_to(head, tail, node, tmp, 0);
			break;
		}
		else
			tmp = tmp->next;
	}
}
