#include "pool.h"

struct  _SimplePool * CreateSimplePoll(size_t cellsize, size_t segsize)
{
	size_t segsize = cellsize * segsize;

	int ret = 0;
	struct _SimplePool * shp = (struct _SimplePool *)malloc(segsize);

	if(shp == NULL)
		return NULL;

	memset(shp, 0, segsize);
	
	ret = pthread_mutex_init(&(shp->lock_), NULL);
	if(ret)
	{
		printf("Pthread_mutex_init error\n");
		free(shp);
		return NULL;
	}

	return shp;
}

int _DestroySimplePool(struct _SimplePool * shp)
{
	int ret = 0;
	if(shp)
	{
		ret = pthread_mutex_destroy(&shp->lock_);
		if(ret)
		{
			printf("pthread_mutex_destroy error");
			return -1;
		}
		free(shp);
		return 0;
	}
	return -1;

}

int __AllocateSegment(struct _SimplePool * shp)
{

	/* first allocate a segment for simple pool, and insert 
	 * it into shp                                        */

	/* the size of a segment including the header length of*
	 * segment and the cells part in this segment          */

	size_t segSize = sizeof(struct _SimplePoolSegment) + 
		             (shp->cellsize_ * SHPCELLNUM);
	struct _SimplePoolSegment * segp = NULL;
	segp = (struct _SimplePoolSegment * )malloc(segSize);
	
	if(segp == NULL)
	{
		return -1;/* allocate segment memory error;*/
	}
	memseg(segp, 0, segSize);

	/* init the newly allocated segment */
	segp->avinum_ = SHPCELLNUM;
	segp->segavibegin_ = (char *)segp + sizeof(struct _SimplePoolSegment);
	segp->segaviend_ = (char *)segp->segavibegin_ + SHPCELLNUM * shp->cellsize_;
	/* segavicursor_ points the first cell in this segment initialily*/
	segp->segavicursor_ = segp->segavibegin_;

	
	/* insert this newly allocated segment into the segment *
	 * list of simple pool. Insert it from the head*/
	segp->segnext_ = shp->seghead_;
	if(shp->seghead_ != NULL)
		shp->seghead_->segprev_ = segp;
	else
		/* There is no segment in this list, so update tail*/
		shp->segtail_ = segp;
	shp->seghead_ = segp;

	/* insert this newly allocated segment into the avialiable
	 * segment list of simple pool. Insert it from the head as
	 * well.*/
	segp->avinext_ = shp->avihead_;
	if(shp->avihead_ != NULL)
		shp->avihead_->aviprev_ = segp;
	else
		/* There is no segment in the avi list, update tail*/
		shp->avitail_ = segp;
	shp->avihead_ = segp;

	return 0;
}









