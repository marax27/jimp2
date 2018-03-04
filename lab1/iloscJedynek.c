#include <stdio.h>

// iloscJedynek(n) - zwraca ilosc bitow o wartosci jeden dla podanego argumentu.
unsigned char iloscJedynek(unsigned long int value){
	int bits = sizeof(value) * 8;
	unsigned char result = 0;
	for(int i=0; i!=bits; ++i){
		if(value & 1)
			++result;
		value >>= 1;  //opcjonalnie value /= 2
		if(!value)
			break;
	}
	return result;
}

int main(void){
	unsigned long int v;
	printf("Podaj liczbe calkowita: ");
	if(scanf("%lu", &v) != 1){
		fputs("Error!\n", stderr);
		return 1;
	}
	printf("iJ(%lu) == %d\n", v, iloscJedynek(v));
}