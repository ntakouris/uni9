#include <stdio.h>
#include <math.h>

int main(void)
{
	
	printf("Fibonacci> ");
	int n;
	scanf("%d", &n);

	if(n < 0){
		return 0;
	}else if(n > 20){
		n = 20;
	}

	int fibo[n];
		
	int f0 = 0;
	int f1 = 1;

	fibo[0] = f0;
	if(n > 0){
		fibo[1] = f1;
		if(n > 1){
			int fpp = f0;//f previous previous (n-2)
			int fp = f1;// f previous (n-1)
			int i;
			for(i=2;i<=n;i++){
				int f = fp + fpp;
				fpp  = fp;
				fp = f;
				fibo[i] = f;
			}
		}
	}

	int numbersPerLine = 1;

	int i;
	for(i=0;i<=n;i++){
		printf("%d", fibo[i]);

		if(numbersPerLine == 5){
			printf("\n");
			numbersPerLine = 1;
		}else{
			printf("\t");
			numbersPerLine++;
		}
	}
	
	printf("\n");

	return 0;
}
