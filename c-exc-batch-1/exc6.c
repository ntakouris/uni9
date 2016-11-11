#include <stdio.h>
#include <math.h>

int recursive(int, int);
float fi(float);
float fii(float);

int main(void)
{
	printf("n for equation 1> ");
	int n;
	scanf("%d", &n);

	printf("a0 for equation 1> ");
	int a0;
	scanf("%d", &a0);
	
	if(n >= 0){
		printf("value> %d", recursive(n, a0));
	}else{
		printf("Negative n is not valid");
	}
	printf("\n");	

	//i
	printf("x for fi> ");
	float x;
	scanf("%f", &x);
	printf("fi> %f\n", fi(x));

	printf("x for fii> ");
	scanf("%f", &x);
	printf("fii> %f\n", fii(x));

	return 0;
}

int recursive(int n, int a0){

	if(n == 0){
		return a0;
	}
	int prev = recursive(n-1, a0);
	return prev * prev * prev * prev * prev  - prev;
}

float fi(float x){
	return pow(x, 5) - pow(x, 3) + 3 * x;
}

float fii(float x){
	return exp(x) + 4 * log(x) - pow(x,2);
}
