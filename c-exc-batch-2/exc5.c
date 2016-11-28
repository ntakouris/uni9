#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void swap(float * x , float * y){
    float temp = *x;
    *x = *y;
    *y = temp;
}

void print(float * arr){
    int c;
    for ( c = 0 ; c <30 ; c++ ){
     printf("%f - ", arr[c]);
    }
    printf("\n");
}

int main()
{
  float array[30];
  int c, d;
  printf("Enter 30 floats\n");
 
  for (c = 0; c < 30; c++){
    scanf("%f", &array[c]);
  }
 
  for (c = 0 ; c < ( 30 - 1 ); c++)
  {
    for (d = 0 ; d < 30 - c - 1; d++)
    {
      if (array[d] > array[d+1])
      {
        swap(&array[d], &array[d+1]);
        print(array);
      }
    }
  }
 
  printf("Sorted list in ascending order:\n");
 
    print(array);
 
  return 0;
}