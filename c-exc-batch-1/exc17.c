#include <stdio.h>
#include <math.h>

void printRow(int);

int main(void)
{
	//print header
	printf("N\t2*N\t3*N\t10*N\n");

	printRow(1);
	printRow(2);
	printRow(3);

	printf("...\t...\t...\t...\n");

	printRow(10);

	return 0;
}

void printRow(int n){
	printf("%d\t%d\t%d\t%d\n", n , n * 2 , n * 3 , n * 10);
}
