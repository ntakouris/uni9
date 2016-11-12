#include <stdio.h>
#include <math.h>

int main(void)
{
	int pin;
	do{
		printf("PIN: ");
		scanf("%d", &pin);
	}while(pin != 2014);
	
		int balance = 100;
		while(1){
			printf("1) View balance\n");
			printf("2) Deposit money\n");
			printf("3) Withdray money\n");
			printf("4) Exit\n");
			printf("> ");
			int code;
			scanf("%d", &code);

			if(code == 1){
				printf("Current acount balance: %d\n\n", balance);
			}else if(code == 2){
				printf("How much money do you want to deposit? ");
				int money;
				scanf("%d", &money);
				if(money > 0){
					balance += money;
					printf("Success! Current account balance: %d\n\n", balance);
				}else{
					printf("Money amount to deposit can't be negative or 0!\n\n");
				}
			}else if(code == 3){
				printf("How much money do you want to withdraw? ");
				int money;
				scanf("%d", &money);
				if(money >= 20 && money % 20 == 0){
					if(balance >= money){
						balance -= money;
						printf("Success! Current account balance: %d\n\n", balance);
					}else{
						printf("You have not got that much money!\n\n");
					}
				}else{
					printf("Money amount to withday can't be negative, 0 or non multiple of 20!\n\n");
				}
			}
		}
	
	return 0;
}
