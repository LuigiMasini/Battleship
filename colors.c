#include <stdio.h>

/*
void main(){
	for (int i=0; i<0xff;i++){
		for (int j=0; j<0xff;j++){
			printf("\e[%d;%dm%4d", i, j);
		}
		printf("\n");
	}
}*/

int main(void)

{
	
	int i, j, n;
	
	
	
	for (i = 0; i < 11; i++) {
		
		for (j = 0; j < 10; j++) {
			
			n = 10*i + j;
			
			if (n > 108) break;
			
			printf("\033[%dm %3d\033[m", n, n);
			
		}
		
		printf("\n");
		
	}
	
	return (0);
	
}
