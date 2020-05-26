#include <stdbool.h>
#include <string.h>

#pragma execution_character_set( "UTF-8" )


#define ROWNUM 12
#define COLNUM 12
#define CELL_SIZE 4	//number of byte for ship piece char, 4 is max UTF-8 size, maybe more in future for ansi colors

typedef enum {e, s, o, n} direction_t;
typedef enum {A=1, B, C, D, E, F, G, H, I, L, M, N} coordinate_t;
typedef enum {nave_5=5, nave_3=3, nave_2=2, nave_1=1} length_t;	//con le lettere fa schifo ma serve identifier

typedef struct {
	length_t length;
	direction_t direction;
	coordinate_t x;
	coordinate_t y;
	
	//inutile, basta vedere se tutti i primi length pezzi colpiti sono true
// 	bool affondata=false;
	
	char renderedShip[5][4]; //4 byte UTF-8 * 5 ship pieces
	
	bool pezzi_colpiti[5];
} barca_t;

void setupBarche(barca_t (*barche)[10], int n){
	int i, j;
	for (i=0; i<n; i++){
		memset((*barche)[i].pezzi_colpiti, false, sizeof(bool)*5);
		memset((*barche)[i].renderedShip, '\0', 20);
	}
}

/* 1 da 5
 * 2 da 3
 * 3 da 2
 * 4 da 1
 */

// "┌", "─", "┬", "┐", "├", "┘", "┤", "┼", "│", "└", "┴"

/*
const char *grid[] = {
"┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤\n",
"│   │   │   │   │   │   │   │   │   │   │   │   │\n",
"└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘\n"
};
*/

/*
║⇐⇑⇓⇔⇕⇳⫿▯⌷═⇒⬍⬌⬇❙❚⬆⬅➡❚➖

✗✘✖❌✕❎‖
░ ▒ ▓

*/

const char*  centralEmptyBlock = "⬜";
const char*  centralFullBlock  = "⬛";
const char*  topEmptyArrow     = "△";
const char*  topFullArrow      = "▲";
const char*  rightEmptyArrow   = "▷";
const char*  rightFullArrow    = "▶";
const char*  bottomEmptyArrow  = "▽";
const char*  bottomFullArrow   = "▼";
const char*  leftEmptyArrow    = "◁";
const char*  leftFullArrow     = "◀";

#ifdef WINDOWS
	#include <cstdio>
	#include <windows.h>
	#include <Errhandlingapi.h>
#endif
void setUp(){
	#ifdef WINDOWS
	if (IsValidCodePage(CP_UTF8))
	{
		// We're all good, so set the console code page...
		if (!SetConsoleOutputCP(CP_UTF8))
		{
			// An error occurred; handle it. Call GetLastError() for more information.
			// ...
			
			char *Err = GetLastError();
		}
	}
	#endif
}

void renderShip(barca_t *barca){
	int i;
	char ship[barca->length][4], *ch;
	memset(*ship, 0, sizeof(ship));
	
	switch(barca->length){
		case 0:
			return;
		case 1:
			ch = barca->pezzi_colpiti[0] ? centralFullBlock : centralEmptyBlock;
			strcpy(ship[0], ch);
			break;
		default:
			for (i=0; i<barca->length; i++){
				if (i==0){
					ch = barca->direction==n || barca->direction==s ? (barca->pezzi_colpiti[i] ? topFullArrow : topEmptyArrow ) : (barca->pezzi_colpiti[i] ? leftFullArrow : leftEmptyArrow );
				}
				else if (i+1 == barca->length){
// 					ch = barca->direction==n || barca->direction==s ? (barca->pezzi_colpiti[i] ? bottomFullArrow : bottomEmptyArrow ) : (barca->pezzi_colpiti[i] ? rightFullArrow : rightEmptyArrow );
					ch = barca->pezzi_colpiti[i] ? centralFullBlock : centralEmptyBlock;
				}
				else {
					ch = barca->pezzi_colpiti[i] ? centralFullBlock : centralEmptyBlock;
				}
				strcpy(ship[i], ch);
			}
	}
	
	for (i=0; i<barca->length; i++){
		strcpy(barca->renderedShip[i], ship[i]);
	}
}

#include <stdio.h>
#include <stdlib.h>

void printShips(barca_t (*barche)[10], int nb){
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
	
	char celle[ROWNUM][COLNUM][4];
	memset(**celle, 0, ROWNUM*COLNUM*4);
	
	for (b=0;b<nb;b++){
		for (i=0; i<(*barche)[b].length; i++){
			int x = (*barche)[b].x;
			int y = (*barche)[b].y;
			
			switch ((*barche)[b].direction){
				case e:
					x+=i;
					break;
				case o:
					x-=i;
					break;
				case n:
					y-=i;
					break;
				case s:
					y+=i;
					break;
				default:
					fprintf(stderr, "!!Error: invalid rotation for %d° ship", b);
			}
			
			strcpy(celle[y][x], (*barche)[b].renderedShip[i]);
		}
	}
	
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
