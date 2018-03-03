#include <stdio.h>

int main(){
	int h;
	printf("Podaj h: ");
	if(scanf("%d", &h) != 1){
		perror("Blad");
		return 1;
	}

	int mw = 2*h - 1;  //start width
	if(h > 0){
		for(int i=0; i<h; ++i){
			int w = mw - 2*i;

			// Option 1.
			/*char *buffer = (char*)malloc(mw + 1);
			buffer[mw] = '\n';
			memset(buffer, ' ', mw);
			memset(buffer+(mw-w)/2, '*', w);
			printf("%s", buffer);*/

			// Option 2.
			printf("% *c", (mw-w)/2 + 1, '\0');
			printf("%0*d\n", w, 0);
		}
	}
}
