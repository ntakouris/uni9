#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct Transaction{
	char name[30];
	int money;
	char date[10];
} Transaction;

int main(void){

    int n = 0;
	printf("N > ");
	scanf("%d", &n);
	Transaction *transactions =  malloc(n * sizeof(Transaction));
	getchar();
	int i;
	for(i=0;i<n;i++){
		printf("Name> ");
		char name[30];
		
		fgets(name, 30, stdin);
		strcpy((transactions+i)->name, name);
		printf("money> ");
		int money;
		scanf("%d", &money);
		(transactions+i)->money = money;
		char date[10];
		getchar();
		printf("Date> ");
		fgets(date, 10 , stdin);
		strcpy((transactions+i)->date, date);
		getchar();
		getchar();
	}

	char name[30];
	printf("Name to search> ");
	fgets(name, 30 , stdin);

	int totalMoney=0;
	int totalTransactions=0;

	for(i=0;i<n;i++){
		if(strcmp((transactions+i)->name, name) == 0){
			printf("Money: %d, Date: %s", (transactions+i)->money, (transactions+i)->date);

			totalMoney+=(transactions+i)->money;
			totalTransactions++;
		}
	}

	if(totalTransactions > 1){
		printf("\nTotal Money: %d\n", totalMoney);
	}

    return 0;
}
