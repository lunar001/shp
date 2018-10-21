# shp
a simple memory pool

Compare with some other memory pool, this memory pool can easily get the address scope of this simple pool. So it can easily
determine whether a piece of memory is allocated from this pool. 

The structure of this simple heap pool is bellow:
	@1. shp is constructed by many segments, evey segment includes serval cells
	@2. all segments are orgnized by rbtree, ordered by the begining address of segment
	@3. if a segment has available cells, this segment will be inserted into the available list
	@4. when allocate a cell from the simple heap pool, get a available segment from available list and get one cell from this segment
	+--------------+
	|   rbroot_  --|-->rbreee to organize all segments
	+--------------+   +-----------+   +-----------+   +-----------+   +-----------+   +-----------+
	|   avihead_ --|-->| avinext_--|-->| avinext_--|-->|           |-->| avinext_--|-->|   NULL    |
	+--------------+   +-----------+   +-----------+   |           |   +-----------+   +-----------+
	|   avitail_   |   | aviprev_  |<--|--aviprev_ |<--|   ...     |<--|--aviprev_ |
	+--------------+   +-----------+   +-----------+   |           |   +-----------+
	                   |    ...    |   |    ...    |   |           |   |    ...    |
			  		   +-----------+   +-----------+   +-----------+   +-----------+

usage:
    The simple heap pool supports for multi-thread.
	Application need to create the pool before use and destory it after use
	Invoke CreateSimplePool() to create a simple heap pool, application should assign the cell size and how many cells in one segment
	Invoke GetCellFromSHP()  to allocate one cell from heap pool.
	Invoke FreeCellToSHP()   to free one cell to heap pool.
	Invoke GetShpAddrScope() to get the address scope of this shp pool.
	Invoke IsAddrInSHP() to test a address is in this scope or not.
