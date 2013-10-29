#include <stdlib.h>
#include <time.h>
#include <sys/types.h>


void main(void)
{
	unsigned seed=1;
	time_t *tp;

	float x1;
	int n1;

	srand(time(tp));

	x1=rand() / 32768.0;
	
	n1=1+(int)(40 * x1);

	printf("%d \n", n1);
}


