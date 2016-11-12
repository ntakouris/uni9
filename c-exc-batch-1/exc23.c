#include <stdio.h>
#include <math.h>

int main(void)
{
	int nums[4];
	int i;
	for(i=0;i<4;i++){
		printf("number %d: ", i + 1);
		scanf("%d", &nums[i]);
	}

	int max1 = nums[0];
	int max2 = nums[1];
	
	int max = max1 + max2;
	
	for(i=1;i<4-1;i++){
		int a = nums[i];
		int j;
		for(j=i+1;j<4;j++){
			int b = nums[j];
			int sum = a+ b;
			if(sum > max){
				max = sum;
				max1 = a;
				max2 = b;
			}
		}
	}	

	printf("Max sum of 2 numbers: %d + %d = %d\n", max1 , max2, max);

	return 0;
}
