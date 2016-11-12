#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("N >0 and <10: ");
	int n;
	scanf("%d", &n);

	if(n > 0 && n < 10){
		int sum = 0;
		int i;
		for(i = 1; i <= n;i++){
			sum += 4 * i * 4 * i;
		}
		printf("Sum: %d\n", sum);
	}
	return 0;
}
