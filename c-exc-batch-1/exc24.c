#include <stdio.h>
#include <math.h>

int main(void)
{
	int num;
	do{
		printf("Enter 2-digit integer: ");
		scanf("%d", &num);
	}while(!(num >= 10 && num < 100));

	//digit1 is the most significant one
	int digit1 = num / 10;
	int digit2 = num - digit1 * 10;
	
	int num_new = 0;
	//construct the new number
	num_new += digit2;
	num_new += digit2 * 10;
	num_new += digit1 * 100;
	num_new += digit1 * 1000;

	printf("%d\n", num_new);

	return 0;
}
