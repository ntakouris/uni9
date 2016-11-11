#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("Type a double: ");
	double d;
	scanf("%lf", &d);
	float f = (float) d;

	printf("d: %.12lf \n", d);
	printf("f: %.12f \n", f);	
	return 0;
}
