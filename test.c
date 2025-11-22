#include <stdio.h>

int main()
{
	short int a=-1;
	unsigned short int b=(unsigned short int)a;
	printf("%d\t%u\t%d\n",a,b,(-15%10));
	return 0;
}
