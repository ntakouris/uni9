#include <stdio.h>
#include <limits.h>
#include <math.h>

int firstEmtyElementPosition(int*, int);
int getPositionOfElementWithValue(int* , int, int);
 
int main(void)
{
	printf("How many numbers do you wish to write? >");
	int n;
	scanf("%d", &n);
	if(n < 1){
		printf("Number quantity cant be <0, exiting....\n");
		return 0;
	}
	
	int numbers[n];
	int i;
	for(i=0;i<n;i++){
		printf("Please enter the next number > ");
		scanf("%d", &numbers[i]);
	}
	
	int keys[n];
	int values[n];

	for(i=0;i<n;i++){
		keys[i] = INT_MAX;//maxify so we can check for empty elements later
		values[i] = 0;
	}

	for(i=0;i<n;i++){//create one entry per number
		int num = numbers[i];

		if(getPositionOfElementWithValue(keys, n, num) != -1){
			int pos = getPositionOfElementWithValue(keys, n, num);//increment values
			values[pos] = values[pos] + 1;
		}else{	//insert an entry to keys and values
			int pos = firstEmptyElementPosition(keys, n);
			keys[pos] = num;
			values[pos] = 1; 
		}
	}
	
	int maxPos = 0;
	int max = keys[maxPos];
	for(i=1;i<n;i++){
		if(values[i] > values[maxPos]){
			maxPos = i;
			max = keys[maxPos];
		}
	}

	printf("Most frequent number: %d - Appearances: %d\n", max, values[maxPos]);
	
	return 0;
}

int firstEmptyElementPosition(int * array, int size){
	int i;
	for(i=0;i<size;i++){
		if(array[i] == INT_MAX){
			return i;
		}
	}
	return -1;
}

int getPositionOfElementWithValue(int * array, int size, int value){
	int i;
	for(i=0;i<size;i++){
		if(array[i] == (int)value){
			return i;
		}
	}
	return -1;
}
