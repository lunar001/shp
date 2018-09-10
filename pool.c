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

struct _SimplePoolSegment *  __AllocateSegment(struct _SimplePool * shp)
{

	/* allocate a segment for simple pool, and insert 
	 * it into shp                                   */

	/* the size of a segment including the header length of*
	 * segment and the cells part in this segment          */

	size_t segSize = sizeof(struct _SimplePoolSegment) + 
		             (shp->cellsize_ * SHPCELLNUM);
	struct _SimplePoolSegment * segp = NULL;
	segp = (struct _SimplePoolSegment * )malloc(segSize);
	
	if(segp == NULL)
	{
		return NULL;/* allocate segment memory error;*/
	}
	memset(segp, 0, segSize);

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

	return segp;
}

/* Delete a segment segp from the shp*/
int __DeleteSegment(struct _SimplePool * shp, struct _SimplePoolSegment * segp)
{
	/* first delete the segment from the seglist and avi segment list, and then*
	 * free the segment memory */

	/* delete segp from the avi segment list */
	if(segp->avinext_ != NULL)
		segp->avinext_->aviprev_ = segp->aviprev_;
	else
		shp->avitail_ = segp->aviprev_;
	if(segp->aviprev_ != NULL)
		segp->aviprev_->avinext_ = segp->avinext_;
	else
		shp->avihead_ = segp->avinext_;


	/* delete the segment from the seglist */
	if(segp->segnext_ != NULL)
		segp->segnext_->segprev_ = segp->segprev_;
	else
		shp->segtail_ = segp->segprev_;
	if(segp->segprev_ != NULL)
		segp->segprev_->segnext_ = = segp->segnext_;
	else
		shp->seghead_ = segp->segnext_;


	/* free the segment memory*/

	free(segp);
	return 0;
}


void * GetCellFromSHP(struct _SimplePool * shp)
{
	/* get a shp cell from shp */
	int ret = pthread_mutex_lock(&shp->lock_);
	void * cellp = NULL;
	void * retp = NULL;
	struct _SimplePoolSegment * segp = NULL;
	if(ret)
	{
		printf("pthread_mutex_lock error %s\n", strerror(errno));
		return retp;
	}

	/*First get a avialiable segment from segment list */
	If(shp->avihead_ == NULL)
	{
		/* there is no aviliable segment in this shp, allocate a new one*/
		segp = __AllocateSegment(shp);
		if(segp == NULL)
		{
			/* allocate segment failed, return NULL to application */
			printf("There is no enough memory to use\n");
			return NULL;
		}
	}
	else
		segp = shp->avihead_;
	
	// search a free cell from segp->segavicursor_ */



}








