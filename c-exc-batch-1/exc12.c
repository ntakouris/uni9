#include <stdio.h>
#include <math.h>

int main(void)
{
	printf("m for meters to inches , e for euro to drx> ");
	char selection;
	scanf("%c", &selection);

	if(!(selection == 'm' || selection == 'e')){
		printf("Unsupported operation, exiting... \n");
		return 0;
	}

	printf("Quantity to transform> ");
	float quantity;
	scanf("%f", &quantity);

	if(selection == 'm'){
		printf("%f m is %f inches", quantity, quantity * 39.3700787);
	}else if(selection == 'e'){
		printf("%f euro is %f drx", quantity, quantity * 340.75);
	}

	printf("\n");
	
	return 0;
}
