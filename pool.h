#ifndef POOL_HEADER
#define POOL_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#include <string.h>

#include "rbtree.h"


#define SHPCELLNUM   64
#define SHPADDRBEGIN 0x7FFFFFFFFFFFFFFF
#define SHPADDREND   0


/* Structure _pool is the struct of my simple pool */

struct _SimplePool
{
	pthread_mutex_t  lock_;
	size_t cellsize_; // the size of each cell in this pool 

	// indicate how many cell in a segment of simple pool, default it will equals SHPCELLNUM
	size_t segsize_; 
	/*use rb_tree to organize the segment list */
	struct rb_root rbroot_;	
	/*struct _SimplePoolSegment * seghead_;
	struct _SimplePoolSegment * segtail_;*/
	struct _SimplePoolSegment * avihead_;
	struct _SimplePoolSegment * avitail_;
	
	/*shpbegin_ and shpend_ indicates the address scope of shp*/
	/*when new segment alocated or deleted, try to update this two fields*/
	unsigned long shpbegin_; 

	/* shpend_ is not the truly end address of shp*/
	/* it is the begin of the segment of smallest address  */
	unsigned long shpend_; 
};


struct _SimplePoolSegment 
{
	/*struct _SimplePoolSegment * segnext_;
	struct _SimplePoolSegment * segprev_;
	*/
	/* use the address of segment as the key of rb_tree*/
	unsigned long key_;
	struct rb_node rbnode_;
	struct _SimplePoolSegment * avinext_;
	struct _SimplePoolSegment * aviprev_;
	size_t avinum_;
	void * segavicursor_;
	void * segavibegin_;
	void * segaviend_;
	/*reserved is the cell in this segment */
};

struct _SimplePoolCell
{
	struct _SimplePoolSegment * segptr_;
};


struct _SimplePool * CreateSimplePool(size_t cellsize, size_t segsize);
int    DestroySimplePool(struct _SimplePool * shp);
void * GetCellFromSHP(struct _SimplePool * shp);
void   FreeCellToSHP(struct _SimplePool * shp, void * p);
void   GetShpAddrScope(struct _SimplePool * shp, unsigned long* begin,  unsigned long * end);
#endif 
