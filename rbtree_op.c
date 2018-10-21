#include "rbtree.h"
#include "pool.h"
#include "print.h"

void rbtree_insert(struct rb_root * rbroot, struct _SimplePoolSegment * segp )
{
	struct rb_node **tr = &(rbroot->rb_node);
	struct _SimplePoolSegment * tmp = NULL;
	
	struct rb_node * p = NULL;
	while(*tr)
	{
		p = (*tr);
		tmp = rb_entry(*tr, struct _SimplePoolSegment, rbnode_);	
		if(segp->key_ < tmp->key_)
			tr = &(*tr)->rb_left;
		else
			tr = &(*tr)->rb_right;
	}

	rb_link_node(&segp->rbnode_, p, tr);
	rb_insert_color(&(segp->rbnode_), rbroot);
}

void rbtree_erase(struct rb_root * rbroot, struct _SimplePoolSegment * segp)
{
	/*struct rb_node * tr = rbroot->rb_node;
	struct _SimplePoolSegment * tmp = NULL;
	while(tr)
	{
		tmp = rb_entry(tr, struct _SimplePoolSegment, rbnode_);
		if(tmp->key_ == segp->key_)
			break;
		if(segp->key_ < tmp->key_)
			tr = tr->rb_left;
		else
			tr = tr->rb_right;
	}
	if(tr)
		rb_erase(tr, rbroot);*/
	
	/* as we already get the wanted deleted node segp->rb_node_, so directly  */
	/* call rb_erase() to delete it from rb_tree                              */
	rb_erase(&segp->rbnode_, rbroot);
	return;
}

struct _SimplePoolSegment * rbtree_find_next(struct rb_root * rbroot, struct _SimplePoolSegment * current)
{
	struct rb_node * ret = rb_next(&current->rbnode_);
	if(ret == NULL)
		return NULL;
	struct _SimplePoolSegment * retsegp = rb_entry(ret, struct _SimplePoolSegment, rbnode_);
	return retsegp;
}

struct _SimplePoolSegment * rbtree_find_prev(struct rb_root * rbroot, struct _SimplePoolSegment * current)
{
	struct rb_node * ret = rb_prev(&current->rbnode_);
	if(ret == NULL)
		return NULL;
	struct _SimplePoolSegment * retsegp = rb_entry(ret, struct _SimplePoolSegment, rbnode_);
	return retsegp;
}

void __rbtree_trav(struct rb_node * node)
{
	if(node == NULL)
		return ;
	__rbtree_trav(node->rb_left);
	struct _SimplePoolSegment * tmp = NULL;
	tmp = rb_entry(node, struct _SimplePoolSegment, rbnode_);
	log_info("key = %lu\n", tmp->key_);

	__rbtree_trav(node->rb_right);
	return ;
}
