#include "pool.h"
#include "print.h"

void rbtree_insert(struct rb_root * rbtoot, struct _SimplePoolSegment * segp);
void rbtree_erase(struct rb_root * rbtoot, struct _SimplePoolSegment * segp);
void __rbtree_trav(struct rb_node * node);
struct  _SimplePool * CreateSimplePool(size_t cellsize, size_t segsize)
{
	size_t seglength =sizeof(struct _SimplePool);

	int ret = 0;
	struct _SimplePool * shp = (struct _SimplePool *)malloc(seglength);

	if(shp == NULL)
		return NULL;

	memset(shp, 0, seglength);
	
	ret = pthread_mutex_init(&(shp->lock_), NULL);
	if(ret)
	{
		log_error("pthread_mutex_init error\n");	
		free(shp);
		return NULL;
	}
	/* the cell size should include the length of the seg
	 * pointer in the front of the cell */
	shp->cellsize_ = cellsize + sizeof(void *);
	shp->segsize_ = segsize ? segsize : SHPCELLNUM;
	/* rbroot_ in shp is clear to zero, so don't need to */
	/* init it explicitly                                */

	return shp;
}

int DestroySimplePool(struct _SimplePool * shp)
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

	/* use the segp to init key_*/

	segp->key_ = (unsigned long)segp;

	segp->segavibegin_ = (char *)segp + sizeof(struct _SimplePoolSegment);
	segp->segaviend_ = (char *)segp->segavibegin_ + SHPCELLNUM * shp->cellsize_;
	/* segavicursor_ points the first cell in this segment initialily*/
	segp->segavicursor_ = segp->segavibegin_;

	rbtree_insert(&shp->rbroot_, segp);	
    	
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
	log_debug("a segment allocated\n");
	__rbtree_trav((shp->rbroot_).rb_node);
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


	rbtree_erase(&shp->rbroot_, segp);

	/* free the segment memory*/

	free(segp);
	log_debug("a segment freed\n");
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
	if(shp->avihead_ == NULL)
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
	
	/* search a free cell from segp->segavicursor_ */
	cellp = segp->segavicursor_;
	while(((struct _SimplePoolCell *)cellp)->segptr_ != NULL && 
	      cellp < segp->segaviend_)
		cellp = cellp +  shp->cellsize_;
	
	if(cellp == segp->segaviend_)/* error this impossible */
		return NULL;
	else
	{
		retp = cellp + sizeof(void *);
		((struct _SimplePoolCell*)cellp)->segptr_ = segp;
	}
	/* update this segment*/
	segp->avinum_ = segp->avinum_ - 1;
	if (segp->avinum_ == 0)
	{
		/* there is no avialiable cell in this segment, 
		 * delete this segment from the aviliable list*/
		if(segp->aviprev_ != NULL)
			segp->aviprev_->avinext_ = segp->avinext_;
		else
			shp->avihead_ = segp->avinext_;
		if(segp->avinext_ != NULL)
			segp->avinext_->aviprev_ = segp->aviprev_;
		else
			shp->avitail_ = segp->aviprev_;
		/* the segp segment will be insert back to this list, so clear
		 * previous and next pointer */
		segp->avinext_ = segp->aviprev_ = 0;

	}
	else
		segp->segavicursor_ = cellp + shp->cellsize_;
	
	return retp;
}

void FreeCellToSHP(struct _SimplePool * shp, void * retp)
{
	/* get the address of the cell */
	struct _SimplePoolCell * cellp = (struct _SimplePoolCell *)(retp - sizeof(struct _SimplePoolCell));
	struct  _SimplePoolSegment * segp = ((struct _SimplePoolCell*)cellp)->segptr_;

	/*indicate this cell as avaliable*/
	cellp->segptr_ = NULL;

	segp->segavicursor_ = cellp;

	if(segp->avinum_ == 0)
	{
		/* the free makes the segment be the aviliable one
		 * so insert it back to shp avi list from the tail*/
		segp->aviprev_ = shp->avitail_;
		if(shp->avitail_ != NULL)
			shp->avitail_->avinext_ = segp;
		else
			shp->avihead_ = segp;
		shp->avitail_ = segp;

	}
	segp->avinum_ = segp->avinum_ + 1;

	 if(segp->avinum_ == SHPCELLNUM)
		/*If there is no available delete free this segment */
		__DeleteSegment(shp, segp);
}

