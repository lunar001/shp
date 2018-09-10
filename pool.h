#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



#define SHPCELLNUM   64

/* Structure _pool is the struct of my simple pool */

struct _SimplePool
{
	pthread_mutex_t  lock_;
	size_t cellsize_;
	size_t segsize_; // indicate how many cell in a segment of simple pool
	void * seghead_;
	void * segtail_;
	void * avihead_;
	void * avitail_;
};


struct _SimplePoolSegment 
{
	void * segnext_;
	void * segprev_;
	void * avinext_;
	void * aviprev_;
	size_t avinum_;
	void * segavicursor_;
	void * segavibegin_;
	void * segaviend_;
	/*reserved is the cell in this segment */
};

struct _SimplePoolCell
{
	void * segptr_;
};


struct _SimplePool * CreateSimplePool(size_t cellsize, size_t segsize);
int _DestroySimplePool(struct _SimplePool * shp);
void * _GetOneCellFromShp(struct _SimplePool * shp);
void * _FreeOneCellToShp(struct _SimplePool * shp);
 
