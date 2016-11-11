#include <stdio.h>
#include <math.h>

int isGradeValid(float);

int main(void)
{
	printf("Please enter 10 grades (Allowed values 0-10)> \n");
	
	float sum = 0;
	

	int subjectNumber;
	for(subjectNumber = 1; subjectNumber <= 10; subjectNumber++)
	{
		float grade;
		int firstTime = 1;
		do
		{
			if(!firstTime)
			{
				printf("Grade value invalid, must be between 1 and 10, please re-enter grade %d> ", subjectNumber);
			}
			else
			{
				printf("Enter grade %d> ", subjectNumber);
				firstTime = 0;
			}
			scanf("%f", &grade);
		}while(isGradeValid(grade) == 0);
		
		sum += grade;
		printf("\n");
	}

	int median = sum / 10.0;
	if(median < 5)
	{
		printf("Fail!");
	}
	else
	{
		printf("Success!");
	}

	printf("\n");

	return 0;
}

int isGradeValid(float grade){
	if(grade >= 0.0 && grade <= 10.0){ return 1; }
	return 0;
}
