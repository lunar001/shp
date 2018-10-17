#ifndef _COMMON_H_
#define _COMMON_H_
#include "rbtree.h"
struct TestNode{
   struct rb_node rbnode;
   int            key;
};

void * rbtree_init();
int rbtree_destroy( struct rb_root * rbroot);
void rbtree_insert(struct rb_root * rbroot, int key);
void rbtree_erase(struct rb_root * rbroot, int key);

#endif
