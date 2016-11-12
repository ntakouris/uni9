#include <stdio.h>
#include <math.h>

int main(void)
{
	int customerCode;
	do{
		printf("Customer 4-digit code: ");
		scanf("%d", &customerCode);
	}while(!(customerCode >= 1000 && customerCode < 10000));

	int seconds;
	do{
		printf("Seconds: ");
		scanf("%d", &seconds);
	}while(seconds < 0);

	int sms;
	do{
		printf("Sms: ");
		scanf("%d", &sms);
	}while(sms < 0);

	float sum = 12.0;
	float timeCost = seconds * 0.02;
	float smsCost = sms * 0.14;
	
	sum += timeCost;
	sum += smsCost;
	
	float fpa = sum * (23.0 / 100.0);
	
	sum += fpa;

	printf("TMob acount\n");

	printf("---------------------------------\n");
	printf("Customer code\tTime\t\tSMS\t\tFPA\t\t|\tTotal\n");
	printf("%d\t\t%f\t%f\t%f\t|\t%f\n", customerCode, timeCost, smsCost, fpa, sum);
	printf("---------------------------------\n");

	return 0;
}
