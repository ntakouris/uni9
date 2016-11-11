#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("Please write 2 integers\n");
	int a,b;
	printf("Integer a> ");
	scanf("%d", &a);
	printf("Integer b> ");
	scanf("%d", &b);

	int product = a * b;
	if(product > 0 && product < 10)
	{
		printf("%d", a+b);
	}
	else if(product > 100 && product < 1000)
	{
		printf("%d" , a-b);
	}
	else
	{
		printf("Number pair doesnt fall under the circumstances");
	}
	
	printf("\n");
	
	return 0;
}
