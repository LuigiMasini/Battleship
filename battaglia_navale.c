#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#define K 13
#define COLNUM 12
#define ROWNUM 12
#define CELL_SIZE 144
typedef enum {n, s, e, o} direction_t;
typedef enum {A=1, B, C, D, E, F, G, H, I, L, M, N} coordinate_t;
typedef struct {
	enum {lunghezza_1=1} length;
	direction_t direction;
	coordinate_t x;
	coordinate_t y;
}colpo_t;
colpo_t colpo[144];
void griglia_colpi();
int main(){
	int gr[K][K];
	int i,j;
	for(i=0;i<K;i++){
		for(j=0;j<K;j++){
			gr[i][0]=i;
			gr[0][j]=j;
			gr[i][j]=0;
		}
	}
	for(i=0;i<K;i++){
		printf("\n");
		for(j=0;j<K;j++){
			printf("%d\t",gr[i][j]);
		}
		printf("\n");
	}
	griglia_colpi();
}
void griglia_colpi(){
	colpo[0].length=1;
	int gr[K][K],Input,selezione=0;
	int i,j,r,c;
	for(i=0;i<K;i++){
		for(j=0;j<K;j++){
			gr[i][0]=i;
			gr[0][j]=j;
			gr[i][j]=0;
		}
	}
	for(i=0;i<K;i++){
		printf("\n");
		for(j=0;j<K;j++){
			printf("%d\t",gr[i][j]);
		}
		printf("\n");
	}
	char currcolpi=0, colpi=1;
	while (1){
		switch (getch()) {
			case '\033':
				// if the first value is esc
				getch(); // skip the [
				switch(getch()) { // the real value
					case 'A':
						// code for arrow up
						colpo[currcolpi].y--;
						break;
					case 'B':
						// code for arrow down
						colpo[currcolpi].y++;
						break;
					case 'C':
						// code for arrow right
						colpo[currcolpi].x++;
						break;
					case 'D':
						// code for arrow left
						colpo[currcolpi].x--;
						break;
				}
				break;
					case 'R':
					case 'r':
						colpo[currcolpi].direction == 1 ? colpo[currcolpi].direction = 0 : colpo[currcolpi].direction++;
						break;
		}
		printshots(&colpo, colpi);
	}
}
/*int cond_vittoria(){
 *    int i,j;
 *    if(i==0){
 *        printf("Hai perso, riprova sarai più fortunato!");
 *    }
 *    if(j==0){
 *        printf("Il giocatore 1 ha vinto");
 *    }
 * }*/
void printshots(colpo_t (*colpo)[144], int nb){
	int r, c, i, b;
	for (c=0;c<=COLNUM;c++){
		//start row
		switch(c){
			case 0:
				printf("\n┌───");
				break;
			case COLNUM:
				printf("┐\n");
				break;
			default:
				printf("┬───");
		}
	}
	
	char celle[ROWNUM][COLNUM][CELL_SIZE];
	
	shot2grid(colpo, nb, &celle);
	
	for (r=0;r<ROWNUM;r++){
		for (c=0;c<COLNUM;c++){
			printf("│ ");
			printf("%s ", celle[r][c][0] ? celle[r][c] : " " );
			
		}
		printf("│\n");
		
		if (r+1 < ROWNUM){
			//middle row
			for (c=0;c<=COLNUM;c++)
				switch(c){
					case 0:
						printf("├───");
						break;
					case COLNUM:
						printf("┤\n");
						break;
					default:
						printf("┼───");
				}
		}
	}
	
	for (c=0;c<=COLNUM;c++){
		//end row
		switch(c){
			case 0:
				printf("└───");
				break;
			case COLNUM:
				printf("┘\n");
				break;
			default:
				printf("┴───");
		}
	}
}
