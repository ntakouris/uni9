#include <stdio.h>
#include <math.h>

int main(void)
{
	float xa, ya , xb , yb;

	printf("xA: ");
	scanf("%f", &xa);
	printf("yA: ");
	scanf("%f", &ya);

	printf("xB: ");
	scanf("%f", &xb);
	printf("yB: ");
	scanf("%f", &yb);

	float distance = sqrt(pow(xa - xb, 2) + pow(ya - yb , 2));
	printf("Distance: %f\n", distance);	
	
	return 0;
}
