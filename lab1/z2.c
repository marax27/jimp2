#include <stdio.h>

// F(n) = F(n-1) + F(n-2) = 2*F(n-2) + F(n-3)
int fib(int n){
	if(n < 1)
		return 0;
	else if(n == 1 || n == 2)
		return 1;
	else
		return 2*fib(n-2) + fib(n-3);
}

int main(void){
	int n;
	printf("Podaj numer wyrazu: ");
	if(scanf("%d", &n) != 1){
		perror("Blad!");
		return 1;
	}
	printf("fib(%d) = %d\n", n, fib(n));
}

