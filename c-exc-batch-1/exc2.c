#include <stdio.h>
#include <math.h>

int main(void)
{

	int numbersOnLine = 0;
	int num;
	for(num = 1; num <= 100;num++){
		printf("%d\t", num);
		numbersOnLine++;
		if(numbersOnLine == 5)
		{
			printf("\n");
			numbersOnLine = 0;
		}
	}	
	return 0;
}
