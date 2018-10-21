#include <stdio.h>
#include "pool.h"
#include "print.h"
#define NUM 64
int main(int argc, char ** argv)
{
	struct _SimplePool * shp = NULL;
	shp = CreateSimplePool(64, 0);
	int num = 0;
	if(shp == NULL)
	
	{
		printf("Create shp error\n");
		return -1;
	}
	
	
	int i = 0;
	void  **pp = NULL;
	if(argc != 2)
	{
		num = NUM;
	}
	else
		num = atoi(argv[1]);
	printf(" num = %d\n", num);
	pp = (void **) malloc(sizeof(void * ) * num);
	memset(pp, 0, sizeof(void * ) * num);
	for (i = num - 1; i >= 0; i--)
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
	unsigned long begin;
	unsigned long end;
	GetShpAddrScope(shp, &begin, &end);

	log_info("begin = %lu end = %lu\n", begin, end);
	for(i = 0; i < num; i++)
	{
		FreeCellToSHP(shp, pp[i]);


	}
	GetShpAddrScope(shp, &begin, &end);
	log_info("begin = %lu end = %lu\n", begin, end);

	DestroySimplePool(shp);
	return 0;
}
