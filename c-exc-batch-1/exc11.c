#include <stdio.h>
#include <math.h>

int main(void)
{
	float a,b,c;
	printf("a> ");
	scanf("%f",&a);
	printf("b> ");
	scanf("%f", &b);
	printf("c> ");
	scanf("%f", &c);
	
	printf("%fx^2 ", a);

	if(b >= 0){
		printf("+ %fx ", b);
	}else{
		printf("- %fx ", -b);
	}

	if(c >= 0){
		printf("+ %f", c);
	}else{
		printf("- %f", c);
	}

	printf(" = 0\n");

	if(a != 0){

		float d = sqrt((b * b) - (4 * a * c));

		if(d < 0){
			printf("Impossible!\n");
			return 0;
		}

		float x1 = ((-1 * b) + d) / (2 * a);
		float x2 = ((-1 * b) - d) / (2 * a);

		if(x1 == x2){
			printf("Double root: %f" , x1);
		}else{
			printf("x1: %f - x2: %f \n", x1 , x2);
		}
	}else if(a == 0 && b != 0){
		float x = -1 * c / b;
		printf("x: %f\n",x); 
	}else{
		if(c != 0){
			printf("Impossible!\n");
		}else{
			printf("True for every x");
		}	
	}

	return 0;
}
