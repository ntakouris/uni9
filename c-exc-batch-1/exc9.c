#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("Farenheit: ");
	float far;
	scanf("%f", &far);

	float cel = (5.0 / 9.0) * (far - 32.0);
	printf("Celcius: %f\n", cel);	
	return 0;
}
