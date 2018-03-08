#include <stdio.h>

int main(void){
	double maximum, current;
	int i = 1;
	while(1){
		printf("Liczba #%d: ", i++);
		if(scanf("%lf", &current) != 1){
			if(i){
				puts("Koniec.");
				break;
			}
			else{
				printf("Brak liczb.");
				return 0;
			}
		}

		if(!i || maximum < current)
			maximum = current;
	}

	printf("Max number is %lf.\n", maximum);
}
