#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#include <string.h>



#define SHPCELLNUM   64

/* Structure _pool is the struct of my simple pool */

struct _SimplePool
{
	pthread_mutex_t  lock_;
	size_t cellsize_;
	size_t segsize_; // indicate how many cell in a segment of simple pool
	struct _SimplePoolSegment * seghead_;
	struct _SimplePoolSegment * segtail_;
	struct _SimplePoolSegment * avihead_;
	struct _SimplePoolSegment * avitail_;
};


struct _SimplePoolSegment 
{
	struct _SimplePoolSegment * segnext_;
	struct _SimplePoolSegment * segprev_;
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
int _DestroySimplePool(struct _SimplePool * shp);
void * _GetOneCellFromShp(struct _SimplePool * shp);
void * _FreeOneCellToShp(struct _SimplePool * shp);
 
