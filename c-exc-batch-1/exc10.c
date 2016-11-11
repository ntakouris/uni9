#include <stdio.h>
#include <math.h>

int main(void)
{
	int x, y;
	printf("Enter number x> ");
	scanf("%d", &x);
	printf("Enter number y>");
	scanf("%d", &y);
	
	printf("Sum: %d\n", x+y);
	if(x >=y){
		printf("X is the greatest\n");
	}else{
		printf("Y is the greatest\n");
	}	
	
	printf("xy: %d\n", x * y);
	
	printf("Log10(X) = %lf \nLog10(Y) = %lf\n", log10(x) , log10(y));

	return 0;
}
