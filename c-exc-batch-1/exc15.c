#include <stdio.h>
#include <math.h>

int fiveDigitPositiveInteger(int);

int main(void)
{
	int num;
	do{
		printf("Enter a 5-digit positive integer: ");
		scanf("%d", &num);
	}while(!fiveDigitPositiveInteger(num));
	
	int rev_num = 0;

	while(num != 0){
		rev_num = rev_num * 10 + num % 10;
		num = num/10;
	}

	printf("Reversed number: %d\n", rev_num);
	
	return 0;
}

int fiveDigitPositiveInteger(int a){
	if(a >= 10000 && a < 100000){ return 1;}
	return 0;
}
