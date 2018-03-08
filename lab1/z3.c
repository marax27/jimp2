#include <stdio.h>

int main(void){
	/*int n;
	printf("Podaj ilosc liczb do wczytania: ");
	if(scanf("%d", &n) != 1){
		perror("Blad!");
		return 1;
	}

	if(n <= 0){
		perror("Niedodatnia ilosc.");
		return 2;
	}*/

	double maximum, current;
	int i = 1;
	//for(int i=0; i!=n; ++i){
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
