#include <stdio.h>
#include <math.h>

int main(void)
{
	
	int charsInLine = 1;

	char currentChar;
	for(currentChar = 48; currentChar<=126 ; currentChar++){
		printf("%c(%d)", currentChar, currentChar);

		if(charsInLine == 10){
			printf("\n");
			charsInLine = 1;
		}else{
			printf("\t");
			charsInLine++;
		}
	}	

	return 0;
}
