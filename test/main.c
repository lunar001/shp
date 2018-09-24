#include <stdio.h>
#include "pool.h"

int main()
{
	struct _SimplePool * shp = NULL;
	shp = CreateSimplePool(64, 64);
	if(shp == NULL)
	
	{
		printf("Create shp error\n");
		return -1;
	}

	int i = 0;
	void *pp[10] = {NULL};
	for (i = 0; i < 10; i++)
	{
		void * p = NULL;
		p = GetCellFromSHP(shp);
		if(p == NULL)
		{
			printf("Get cell from pool error\n");
			return -1;
		}
		memset(p, 0, 64);
		pp[i] = p;
	}

	for(i = 0; i < 10; i++)
	{
		FreeCellToSHP(shp, pp[i]);
	}

	DestroySimplePool(shp);
	return 0;
}
