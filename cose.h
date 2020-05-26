#include <stdbool.h>
#include <string.h>

#pragma execution_character_set( "UTF-8" )

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

const char *cellsContent = "│ %s │ %s │ %s │ %s │ %s │ %s │ %s │ %s │ %s │ %s │ %s │ %s │";
const char *topRow     = "┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐";
const char *emptyCells = "│   │   │   │   │   │   │   │   │   │   │   │   │";
const char *middleRow  = "├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤";
const char *bottomRow  = "└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘";

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

void printShips(barca_t (*barche)[10], int n){
	int r, b, i;
	printf("\n%s\n",topRow);
	
	/*
	length_t fatti[n];
	memset(fatti, 0, n);
	*/
	
	char celle[12][12][4];
	memset(**celle, 0, 12*12*4);
	
	for (b=0;b<n;b++){
		for (i=0; i<(*barche)[b].length; i++){
			int x = (*barche)[b].x;
			int y = (*barche)[b].y;
			
			switch (*barche)[b].direction){
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
	
	
	
	for (r=0;r<12;r++){
		char pezziInRiga[12][4];
		memset(*pezziInRiga, 0, 12*4);
		
		/*
		//NOTE inefficiente
		
		for (b=0;b<n;b++){
			if (fatti[b] == (*barche)[b].length ){
// 				printf("%d %d", fatti[b], (*barche)[b].length);
				break;
			}
			
			if ( (*barche)[b].direction == e || (*barche)[b].direction == o ){
				//se orizzontale
				if ( (*barche)[b].y == r )
					//e sulla riga corrente
					for (i=0;i<(*barche)[b].length;i++){
						//copia ogni pezzo nella cella giusta in pezzi in riga
						strcpy(pezziInRiga[(*barche)[b].x + ( (*barche)[b].direction == e ? i : -i)], (*barche)[b].renderedShip[i]);
						fatti[b]++;
					}
			}
			else{
				int delta = (*barche)[b].y - r;
				if ( abs(delta) <= (*barche)[b].length ){
					//potrebbe, a seconda della direzione, avere un pezzo in row
					
					if (delta == 0 || ( (*barche)[b].direction == s && delta > 0 ) || ( (*barche)[b].direction == n && delta < 0) ){
						strcpy(pezziInRiga[(*barche)[b].x],  (*barche)[b].renderedShip[abs(delta)]);
						fatti[b]++;
					}
				}
			}
			
		}
		*/
		
		for (i=0;i<12;i++){
			printf("│ ");
			printf("%s ", pezziInRiga[i][0] ? pezziInRiga[i] : " " );
			
		}
		printf("│\n");
		
		if (r < 11)
			printf("%s\n", middleRow);
	}
	printf("%s\n",bottomRow);
}
