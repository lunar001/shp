#include "common.h"
#include <string.h>
#include <stdlib.h>
void * rbtree_init()
{
    struct rb_root * rbroot = NULL;
    rbroot = (struct rb_root*)malloc(sizeof(struct rb_root));
    if(rbroot == NULL)
       return NULL;
    memset(rbroot, 0, sizeof(struct rb_root));
    return rbroot;
}

int rbtree_destroy(struct rb_root* rbroot)
{
   if(rbroot == NULL)
      return -1;
   if(rbroot->rb_node != NULL)
       return -1;
   free(rbroot);
   return 0;    
}

void rbtree_insert(struct rb_root* rbroot, int key)
{
    struct TestNode * new = NULL;
    struct rb_node ** tr = &(rbroot->rb_node);
    struct rb_node * p = NULL;
    struct TestNode * tmp = NULL;
    new = (struct TestNode *)malloc(sizeof(struct TestNode));
    memset(new, 0, sizeof(struct rb_node));
    new->key = key;
    while(*tr)
    {
        p = *tr;
        tmp = rb_entry(*tr,struct TestNode, rbnode);
        if(tmp->key < new->key)
           tr =&( (*tr)->rb_right);
        else
           tr =&((* tr)->rb_left);  
    }
    
    rb_link_node(&(new->rbnode), p, tr);
    rb_insert_color(&(new->rbnode),rbroot);
}

void  rbtree_erase(struct rb_root* rbroot, int key)
{
    struct TestNode * tmp = NULL;
    struct rb_node * tr = rbroot->rb_node;
    while(tr)
    {
        tmp = rb_entry(tr, struct TestNode, rbnode); 
        if(tmp->key == key)
            break;
        if(tmp->key < key)
            tr = tr->rb_right;
        else 
            tr = tr->rb_left;
    }
    if(tr)
        rb_erase(tr, rbroot);
    if(tmp)
        free(tmp);
    return ;
}

void __rbtree_trav(struct rb_node * node)
{
    if(node == NULL)
        return ;
    __rbtree_trav(node->rb_left);
    
    struct TestNode * tmp = NULL;
    tmp = rb_entry(node, struct TestNode, rbnode);
    printf(" key = %d ", tmp->key);

    __rbtree_trav(node->rb_right);
    return ;
}
void rbtree_trav(struct rb_root * rbroot)
{
    __rbtree_trav(rbroot->rb_node);
    return ;
}

