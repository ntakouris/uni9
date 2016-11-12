#include <stdio.h>
#include <math.h>

int main(void)
{
	int n;
	do{
		printf("n >= 1: ");
		scanf("%d", &n);
	}while(n <= 0);

	printf("Athrisma %d  prwtwn tetragwnwn\n", n);
	
	int sum = 0;

	int i;
	for(i=1;i<=n;i++){
		sum += i * i;
	}

	printf("with a for loop: %d\n", sum);
	
	sum = 0;

	i = 1;
	while(i <= n){
		sum += i * i;
		i++;
	}
	
	printf("with a while loop: %d\n", sum);

	sum = 0;
	
	i = 1;

	do{
		sum += i * i;
	}while(++i <= n);

	printf("with a do..while loop: %d\n", sum);

	printf("%d prwtes dynameis\n", n);

	sum = 0;
	for(i=1;i<=n;i++){
		sum += pow(i,i);
	}

	printf("with a for loop: %d\n", sum);

	sum = 0;
	i = 1;
	
	while(i <= n){
		sum += pow(i, i);
		i++;
	}

	printf("with a while loop: %d\n", sum);

	sum = 0;
	i = 1;

	do{
		sum += pow(i, i);
	}while(++i <= n);

	printf("with a do..while loop: %d\n", sum);

	return 0;
}
