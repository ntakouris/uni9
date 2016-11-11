#include <stdio.h>
#include <math.h>

int main(void)
{
	float grade = 5.5;
	scanf("%f", &grade);

	if(grade >=0){
		printf(">=0");
	}	
	if(grade <= 10){
		printf("<=10");
	}

	return 0;
}
