#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student{
	char firstName[30];
	char lastName[30];

	int grade;
	int times;
	int year;
} Student;

int main(void){

	int n = 3;
	
	Student * students = malloc(n * sizeof(Student));

	int i = 0;
	for(i=0;i<n;i++){
		printf("First Name> ");
		fgets((students+i)->firstName , 30 , stdin);

		printf("Last Name> ");
		fgets((students+i)->lastName, 30 , stdin);

		printf("Grade> ");
		scanf("%d", &(students+i)->grade);
		
		printf("Times> ");
		scanf("%d", &(students+i)->times);
		
		printf("Year> ");
		scanf("%d", &(students+i)->year);
		
		getchar();
	}

	int biggestGrade = 0;
	int greatestYear = 0;
	int mostTime = 0;
	for(i=0;i<n;i++){
		if((students+i)->grade > biggestGrade){
			biggestGrade = (students+i)->grade;
		}
		if((students+i)->year > greatestYear){
			greatestYear = (students+i)->year;
		}
		if((students+i)->times > mostTime){
			mostTime = (students+i)->times;
		}
	}

	printf("Biggest grade: %d\n", biggestGrade);
	
	for(i=0;i<n;i++){
		if((students+i)->grade == biggestGrade){
			printf("%s",(students+i)->firstName);
			printf("%s",(students+i)->lastName);
			printf("-----------------\n");
		}
		
	}

	printf("Greatest year: %d\n", greatestYear);

	for(i=0;i<n;i++){
		if((students+i)->year == greatestYear){
			printf("%s",(students+i)->firstName);
			printf("%s",(students+i)->lastName);
			printf("--------------------\n");
		}
	}

	printf("Most time: %d\n", mostTime);
	
	for(i=0;i<n;i++){
		if((students+i)->times == mostTime){
			printf("%s",(students+i)->firstName);
			printf("%s",(students+i)->lastName);
			printf("---------------------\n");
		}
	}


    return 0;
}
