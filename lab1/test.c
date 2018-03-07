#include <stdio.h>

void pyt1();
void pyt6();
char pyt9_foo(short a);
void pyt11_f(long *a, long *b);
long dec2Bin(long n);

void pyt15_foo(int *p, ...);

/* Pytanie 12. */
int getSize(){return 2*2;}
const int populacja = 1000;
int rozmiar = 12;

int main(void){
	pyt1();
	pyt6();

	/* Pyt. 8
	 * -v = ~v + 1  =>  v = ~(-v - 1)
	 * Example: -( 01001011 ) = 10110100 + 1 = 10110101
	 * 
	 * a = 10100011 = -~10100010 = -01011101 = -(64+16+8+4+1) = -93
	 * b = 00011101 = 16+8+4+1 = 29
	 * a - b = -93 - 29 = -122 = -(01111010) = 10000101 + 1 = 10000110
	 */

	/* Pyt. 11
	 * [3, 4, 7]
	 * f(&1, &1):
	 *    f(&2, &1)
	 *    [3, 4, 8], f(OUT, &1)
	 *    f(OUT, &2)
	 * return: [3, 4, 8]
	 */

	/* Pyt. 12: wszystkie kompiluja sie bez problemu, nawet w ANSI C. */
	int tab[0];
	double fit[populacja];
	char bufor[rozmiar];
	char *napis[getSize()];
}

void pyt1(){
	puts("Pytanie 1:");
	unsigned char a = 0, i;
	for(i=1; i; i <<= 3){
		a ^= i%5;
		printf("\tStep; a: %ld (%d); i: %ld\n", dec2Bin(a), (int)a, dec2Bin(i));
	}
	puts("/1");
}

void pyt6(){
	puts("Pytanie 6:");
	unsigned char x;
	int count = 0;
	for(x=2; x; x+=3){
		printf("%d\t", x);
		++count;
	}
	printf("\n\tCount: %d\n", count);
	puts("/6");
}

char pyt9_foo(short a){
	char i, b = (a > 0) ? 7 : (char)(a % 9);
	for(i = b; i > 0; --i)
		b -= 2;
	return b;
}

long dec2Bin(long n){
	int remainder; 
	long binary = 0, i = 1;

	while(n != 0){
		remainder = n%2;
		n /= 2;
		binary += remainder*i;
		i *= 10;
	}
	return binary;
}

void pyt11_f(long *a, long *b){
	++a;
	++*a;
	b++;
}

void pyt15_foo(int *p, ...){
	puts("Pytanie 15:\n/15");
}
