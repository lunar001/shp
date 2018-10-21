#include <stdio.h>
#include "pool.h"
#include "print.h"
#define NUM 6400 
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

	unsigned long saveaddr = 0;
	
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
	for (i = 0; i < num; i++)
	{
		void * p = NULL;
		p = GetCellFromSHP(shp);
		if(p == NULL)
		{
			printf("Get cell from pool error\n");
			return -1;
		}
		if(i == 64)
		{
			saveaddr = (unsigned long)p;
			log_info("saveaddr = %lu\n", (unsigned long)p);
		}
		memset(p, 0, 64);
		pp[i] = p;
	}
	unsigned long begin;
	unsigned long end;
	GetShpAddrScope(shp, &begin, &end);

	log_info("begin = %lu end = %lu\n", begin, end);
	int result = 0;
	result = IsAddrInShp(shp, saveaddr - 16);

	if(result == 1)
	{
		log_info("saveaddr is in shp\n");
	}
	else
	{	log_info("saveaddr is not in shp\n");
	}


	for(i = 0; i < num; i++)
	{
		FreeCellToSHP(shp, pp[i]);


	}
	GetShpAddrScope(shp, &begin, &end);
	log_info("begin = %lu end = %lu\n", begin, end);

	DestroySimplePool(shp);
	return 0;
}
