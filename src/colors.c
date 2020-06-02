#include <stdio.h>
#include <port.h>

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
	
	printf("\n\n\n");
	
	for(i=0; i<16; i++){
		for (j=0; j<16; j++){
			char c[4];
			sprintf(c, "%x%x", j, i);
			int color=strtol(c, NULL, 16);
			toColor(color);
// 			printf("%s %d, ", c, color);
			
			printf(" %3d", color);
			toColor(7);
		}
		printf("\n");
	}
	
	return (0);
	
}
