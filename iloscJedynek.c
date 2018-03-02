#include <stdio.h>

// iloscJedynek(n) - zwraca ilosc bitow o wartosci jeden dla podanego argumentu.
int iloscJedynek(unsigned long int value){
	int bits = sizeof(value) * 8;
	int result = 0;
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
	scanf("%lu", &v);
	printf("iJ(%lu) == %d\n", v, iloscJedynek(v));
}