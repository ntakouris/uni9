#include <stdio.h>
#include <math.h>

int gcd_algorithm(int x, int y)
{
    if (y == 0) {
        return x;
    } else if (x >= y && y > 0) {
        return gcd_algorithm(y, (x % y));
    }

	return 0;
}

int main(void)
{
	printf("MKD Calculator\n");

	int x,y;
	printf("x> ");
	scanf("%d", &x);
	printf("y> ");
	scanf("%d", &y);

	if(y > x){
		int temp = x;
		x = y;
		y = temp;
	}

	int gcd = gcd_algorithm(x, y); 
    	if (gcd){
        	printf("MKD = %d\n", gcd);
	}else{
        	printf("Invalid input\n");	
	}

	return 0;
}
