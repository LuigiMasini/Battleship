#include <stdbool.h>
#include <string.h>

#define ROWNUM 12
#define COLNUM 12
#define CELL_SIZE 4	//number of byte for ship piece char, 4 is max UTF-8 size + 1 x null terminator
#define NUM_BARCHE 10

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
	
	char renderedShip[5][CELL_SIZE]; //UTF-8 char * 5 ship pieces
	
	bool pezzi_vivi[5];
	
	bool isPosValid;
} barca_t;

typedef struct {
	coordinate_t x;
	coordinate_t y;
} colpo_t;

void setupBarche(barca_t (*barche)[NUM_BARCHE], int n){
	int i;
	for (i=0; i<n; i++){
		(*barche)[i].y=0;
		(*barche)[i].x=0;
		memset((*barche)[i].pezzi_vivi, true, sizeof(bool)*5);
		memset((*barche)[i].renderedShip, '\0', 5*CELL_SIZE);
		(*barche)[i].isPosValid = true;
	}
	
	(*barche)[0].length=5;
	(*barche)[1].length=3;
	(*barche)[2].length=3;
	(*barche)[3].length=2;
	(*barche)[4].length=2;
	(*barche)[5].length=2;
	(*barche)[6].length=1;
	(*barche)[7].length=1;
	(*barche)[8].length=1;
	(*barche)[9].length=1;
}

/* 1 da 5
 * 2 da 3
 * 3 da 2
 * 4 da 1

// "┌", "─", "┬", "┐", "├", "┘", "┤", "┼", "│", "└", "┴"

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

║⇐⇑⇓⇔⇕⇳⫿▯⌷═⇒⬍⬌⬇❙❚⬆⬅➡❚➖

✗✘✖❌✕❎‖🌊╳
░ ▒ ▓

*/

char*  centralEmptyBlock = "◻";		//"⬜";
char*  centralFullBlock  = "◼";		//"⬛";
char*  topEmptyArrow     = "△";
char*  topFullArrow      = "▲";
char*  rightEmptyArrow   = "▷";
char*  rightFullArrow    = "▶";
char*  bottomEmptyArrow  = "▽";
char*  bottomFullArrow   = "▼";
char*  leftEmptyArrow    = "◁";
char*  leftFullArrow     = "◀";
const char*  waterIcon         = "▒";	//"🌊";
const char*  hitIcon           = "╳";	//"❌";

#ifdef WINDOWS
	#include <cstdio>
	#include <windows.h>
	#include <Errhandlingapi.h>

// #include <windowsx.h>
// #include <conio.h>
// #include <mmsystem.h>
#endif

#include<time.h>

void setUp(){
	
	toColor(7);
	
	#ifdef WINDOWS
	
	PlaySound("a.mp3",NULL,SND_ASYNC | SND_LOOP);
	
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
	
	//seed rand w/ time
	srand(time(0));
}

void renderShip(barca_t *barca){
	int i;
	char ship[barca->length][CELL_SIZE], *ch;
	memset(*ship, 0, sizeof(ship));
	
	switch(barca->length){
		case 1:
			ch = barca->pezzi_vivi[0] ? centralFullBlock : centralEmptyBlock;
			strcpy(ship[0], ch);
			break;
		default:
			for (i=0; i<barca->length; i++){
				if (i==0){
					ch = barca->direction==n ?\
					(barca->pezzi_vivi[i] ? bottomFullArrow : bottomEmptyArrow )\
					: barca->direction == s ?\
					(barca->pezzi_vivi[i] ? topFullArrow : topEmptyArrow )\
					: barca->direction == e ? \
					(barca->pezzi_vivi[i] ? leftFullArrow : leftEmptyArrow )\
					: (barca->pezzi_vivi[i] ? rightFullArrow : rightEmptyArrow );
				}
				else if (i+1 == barca->length){
// 					ch = barca->pezzi_vivi[i] ? centralFullBlock : centralEmptyBlock;
					ch = barca->direction==n ?\
					(barca->pezzi_vivi[i] ? topFullArrow : topEmptyArrow )\
					: barca->direction == s ?\
					(barca->pezzi_vivi[i] ? bottomFullArrow : bottomEmptyArrow )\
					: barca->direction == e ? \
					(barca->pezzi_vivi[i] ? rightFullArrow : rightEmptyArrow )\
					: (barca->pezzi_vivi[i] ? leftFullArrow : leftEmptyArrow );
				}
				else {
					//NOTE ottimizzabile:
					//quadrato non cambia direzione, potrei evitare di renderizzarlo ogni volta
					//bisogna capire se costa di più il controllo o la copia
					ch = barca->pezzi_vivi[i] ? centralFullBlock : centralEmptyBlock;
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

void ship2grid (barca_t (*barche)[NUM_BARCHE], int nb, char (*celle)[ROWNUM][COLNUM][CELL_SIZE], bool (*invalid)[ROWNUM][COLNUM]) {
	int b, i;
	
	bool occupato[ROWNUM][COLNUM];
	char occupatoNum[ROWNUM][COLNUM];
	bool currentInvalid = false;
	
	memset(***celle, 0, ROWNUM*COLNUM*CELL_SIZE);
	memset(**invalid, false, ROWNUM*COLNUM*sizeof(bool));
	memset(*occupato, false, ROWNUM*COLNUM*sizeof(bool));
	memset(*occupatoNum, 0, ROWNUM*COLNUM);
	
	for (b=0;b<nb;b++){
		(*barche)[b].isPosValid = true;
		
		//le barche con b maggiore hanno "z-index" maggiore
		renderShip( &(*barche)[b] );
		
		for (i=0; i<(*barche)[b].length; i++){
			int x = (*barche)[b].x;
			int y = (*barche)[b].y;
			
			switch ((*barche)[b].direction){
				case e:
					x+=i;
					if (x>COLNUM-1){
						x-=COLNUM;
						currentInvalid=true;
					}
					break;
				case o:
					x-=i;
					if (x<0){
						x+=COLNUM;
						currentInvalid=true;
					}
					break;
				case n:
					y-=i;
					if (y<0){
						y+=ROWNUM;
						currentInvalid=true;
					}
					break;
				case s:
					y+=i;
					if(y>ROWNUM-1){
						y-=ROWNUM;
						currentInvalid=true;
					}
					break;
				default:
					fprintf(stderr, "!!Error: invalid rotation for %d° ship", b);
			}
			
			//controlla se le celle intorno sono occupate, esclidendo quelle al bordo
			//NOTE si può fare più elegante con macro
			if (\
				(						   occupato[y][x]		&& occupatoNum[y][x]	!= b) || \
				(x-1>=0					&& occupato[y][x-1]	&& occupatoNum[y][x-1]	!= b) || \
				(x+1<=COLNUM-1				&& occupato[y][x+1]	&& occupatoNum[y][x+1]	!= b) || \
				(y-1>=0 && x-1>=0				&& occupato[y-1][x-1]	&& occupatoNum[y-1][x-1]!= b) || \
				(y-1>=0 					&& occupato[y-1][x]	&& occupatoNum[y-1][x]	!= b) || \
				(y-1>=0 && x+1<=COLNUM-1		&& occupato[y-1][x+1]	&& occupatoNum[y-1][x+1]!= b) || \
				(y+1<=ROWNUM-1 && x-1>=0		&& occupato[y+1][x-1]	&& occupatoNum[y+1][x-1]!= b) || \
				(y+1<=ROWNUM-1				&& occupato[y+1][x]	&& occupatoNum[y+1][x]	!= b) || \
				(y+1<=ROWNUM-1 && x+1<=COLNUM-1	&& occupato[y+1][x+1]	&& occupatoNum[y+1][x+1]!= b) ){
				
// 				printf("invalid");
				(*invalid)[y][x]=true;
				currentInvalid=true;
			}
			occupato[y][x]=true;
			occupatoNum[y][x]=b;
			strcpy((*celle)[y][x], (*barche)[b].renderedShip[i]);
		}
		if ( currentInvalid )
			(*barche)[b].isPosValid = false;
	}
}

void printShips(barca_t (*barche)[NUM_BARCHE], int nb, int left){
	int r, c;
	
	printf("\n");
	toColor(7);
	
	for (c=0;c<left;c++)
		printf(" ");
	
	for (c=0;c<=COLNUM;c++){
		//start row
		switch(c){
			case 0:
				printf("┌───");
				break;
			case COLNUM:
				printf("┐\n");
				break;
			default:
				printf("┬───");
		}
	}
	
	char celle[ROWNUM][COLNUM][CELL_SIZE];
	bool invalid[ROWNUM][COLNUM];
	
	ship2grid(barche, nb, &celle, &invalid);
	
	for (r=0;r<ROWNUM;r++){
		for (c=0;c<left;c++)
			printf(" ");
		
		for (c=0;c<COLNUM;c++){
			printf("│ ");
			
			if (invalid[r][c]){
				toColor(0x0c);	//red
			}
			
			printf("%s", celle[r][c][0] ? celle[r][c] : " " );
			
			
			if (invalid[r][c]){
				toColor(0x07);	//reset
			}
			
			printf(" ");
		}
		printf("│\n");
		
		if (r+1 < ROWNUM){
			//middle row
			
			for (c=0;c<left;c++)
				printf(" ");

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
	
	for (c=0;c<left;c++)
		printf(" ");
	
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

int centerText(char *s){
	
	int cols, rows, i;
	getTerminalSize(&cols, &rows);
	
	for (i=0; i<(cols - strlen(s))/2; i++)
		printf(" ");
	
	return printf("%s",s);
}

//https://gsamaras.wordpress.com/code/random-numbers-%E2%88%88min-max/
int randomRange(int min, int max){
	return ( rand() % ( max - min + 1 ) ) + min;
}

bool checkOverlapping (barca_t (*barche)[NUM_BARCHE], int bn) {
	int b, i;
	
	bool occupato[ROWNUM][COLNUM];
	char occupatoNum[ROWNUM][COLNUM];
	
	memset(*occupato, false, ROWNUM*COLNUM*sizeof(bool));
	memset(*occupatoNum, 0, ROWNUM*COLNUM);
	
	for (b=0;b<bn;b++){
		(*barche)[b].isPosValid = true;
		
		for (i=0; i<(*barche)[b].length; i++){
			int x = (*barche)[b].x;
			int y = (*barche)[b].y;
			
			switch ((*barche)[b].direction){
				case e:
					x+=i;
					if (x>COLNUM-1){
						x-=COLNUM;
						(*barche)[b].isPosValid = false;
						return true;
					}
					break;
				case o:
					x-=i;
					if (x<0){
						x+=COLNUM;
						(*barche)[b].isPosValid = false;
						return true;
					}
					break;
				case n:
					y-=i;
					if (y<0){
						y+=ROWNUM;
						(*barche)[b].isPosValid = false;
						return true;
					}
					break;
				case s:
					y+=i;
					if(y>ROWNUM-1){
						y-=ROWNUM;
						(*barche)[b].isPosValid = false;
						return true;
					}
					break;
				default:
					fprintf(stderr, "!!Error: invalid rotation for %d° ship", b);
			}
			
			//controlla se le celle intorno sono occupate, esclidendo quelle al bordo
			//NOTE si può fare più elegante con macro
			if (\
				(						   occupato[y][x]		&& occupatoNum[y][x]	!= b) || \
				(x-1>=0					&& occupato[y][x-1]	&& occupatoNum[y][x-1]	!= b) || \
				(x+1<=COLNUM-1				&& occupato[y][x+1]	&& occupatoNum[y][x+1]	!= b) || \
				(y-1>=0 && x-1>=0				&& occupato[y-1][x-1]	&& occupatoNum[y-1][x-1]!= b) || \
				(y-1>=0 					&& occupato[y-1][x]	&& occupatoNum[y-1][x]	!= b) || \
				(y-1>=0 && x+1<=COLNUM-1		&& occupato[y-1][x+1]	&& occupatoNum[y-1][x+1]!= b) || \
				(y+1<=ROWNUM-1 && x-1>=0		&& occupato[y+1][x-1]	&& occupatoNum[y+1][x-1]!= b) || \
				(y+1<=ROWNUM-1				&& occupato[y+1][x]	&& occupatoNum[y+1][x]	!= b) || \
				(y+1<=ROWNUM-1 && x+1<=COLNUM-1	&& occupato[y+1][x+1]	&& occupatoNum[y+1][x+1]!= b) ){
				
				(*barche)[b].isPosValid = false;
				return true;
			}
			/*
			else
				for (int j= 0; j<COLNUM;j++ ){
					for (int k=0; k<ROWNUM; k++)
							if (occupato[j][k] || true)
								printf("%d ",occupatoNum[j][k] );
							else
								printf("  ");
					printf("\n");
				}
			printf("\n");
			*/
			occupato[y][x]=true;
			occupatoNum[y][x]=b;
		}
	}
	return false;
}

void battleship2grid (barca_t (*barche)[NUM_BARCHE], int nb, char (*celle)[ROWNUM][COLNUM][CELL_SIZE], bool (*colpite)[ROWNUM][COLNUM], int (*nbInCelle)[ROWNUM][COLNUM]) {
	int b, i;
	
	memset(***celle, 0, ROWNUM*COLNUM*CELL_SIZE);
	memset(**nbInCelle, -1, ROWNUM*COLNUM*sizeof(int));
	
	for (b=0;b<nb;b++){
		
		for (i=0; i<(*barche)[b].length; i++){
			int x = (*barche)[b].x;
			int y = (*barche)[b].y;
			
			switch ((*barche)[b].direction){
				case e:
					x+=i;
					if (x>COLNUM-1)
						x-=COLNUM;
					break;
				case o:
					x-=i;
					if (x<0)
						x+=COLNUM;
					break;
				case n:
					y-=i;
					if (y<0)
						y+=ROWNUM;
					break;
				case s:
					y+=i;
					if(y>ROWNUM-1)
						y-=ROWNUM;
					break;
				default:
					fprintf(stderr, "!!Error: invalid rotation for %d° ship", b);
			}
			
			if ((*colpite)[y][x]){
				(*barche)[b].pezzi_vivi[i] = false;
				(*nbInCelle)[y][x] = b;
			}
		}
	}
	
	
	for (b=0;b<nb;b++){
		
		//le barche con b maggiore hanno "z-index" maggiore
		renderShip( &(*barche)[b] );
		
		for (i=0; i<(*barche)[b].length; i++){
			int x = (*barche)[b].x;
			int y = (*barche)[b].y;
			
			switch ((*barche)[b].direction){
				case e:
					x+=i;
					if (x>COLNUM-1)
						x-=COLNUM;
					break;
				case o:
					x-=i;
					if (x<0)
						x+=COLNUM;
					break;
				case n:
					y-=i;
					if (y<0)
						y+=ROWNUM;
					break;
				case s:
					y+=i;
					if(y>ROWNUM-1)
						y-=ROWNUM;
					break;
				default:
					fprintf(stderr, "!!Error: invalid rotation for %d° ship", b);
			}
			strcpy((*celle)[y][x], (*barche)[b].renderedShip[i]);
		}
	}
}

void printCampo(barca_t (*myships)[NUM_BARCHE], barca_t (*hisships)[NUM_BARCHE], colpo_t myshots[COLNUM*ROWNUM], colpo_t hisshots[COLNUM*ROWNUM], int nMyshots, int nHisShots){
	
	int r, c, cols, rows;
	
	getTerminalSize(&cols, &rows);
	
	int left = (cols - (COLNUM*4 + 1)*2 - 1 )/3;
	
	printf("\n");
	toColor(7);
	
	for(r=0;r<2; r++){
		for (c=0;c<left;c++)
			printf(" ");
		
		for (c=0;c<=COLNUM;c++){
			//start row
			switch(c){
				case 0:
					printf("┌───");
					break;
				case COLNUM:
					printf("┐");
					break;
				default:
					printf("┬───");
			}
		}
	}
	printf("\n");
	
	char mycelle[ROWNUM][COLNUM][CELL_SIZE];	//acqua mycellare
	bool mycolpite[ROWNUM][COLNUM];
	int mynbInCelle[ROWNUM][COLNUM];
	memset(*mycolpite, false, ROWNUM*COLNUM*sizeof(bool));
	
	for (r=0;r<nHisShots;r++)
		mycolpite[hisshots[r].y][hisshots[r].x]=true;
	
	battleship2grid(myships, NUM_BARCHE, &mycelle, &mycolpite, &mynbInCelle);
	
	
	char hiscelle[ROWNUM][COLNUM][CELL_SIZE];	//acqua mycellare
	int hisnbInCelle[ROWNUM][COLNUM];
	bool hiscolpite[ROWNUM][COLNUM];
	memset(*hiscolpite, false, ROWNUM*COLNUM*sizeof(bool));
	
	for (r=0;r<nMyshots;r++)
		hiscolpite[myshots[r].y][myshots[r].x]=true;
	
	battleship2grid(hisships, NUM_BARCHE, &hiscelle, &hiscolpite, &hisnbInCelle);
	
	for (r=0;r<ROWNUM;r++){
		//BEGIN le mie barche
		for (c=0;c<left;c++)
			printf(" ");
		
		for (c=0;c<COLNUM;c++){
			printf("│ ");
			
			if (mycolpite[r][c]){
				if (mynbInCelle[r][c] != -1){
					
					//cella è una barca colpita
					
					toColor(0x0c);	//red
					printf("%s", mycelle[r][c]);
				}
				else{
					//cella è acqua colpita
					
					toColor(19);
					printf("%s",waterIcon);
				}
			}
			else
				printf("%s", mycelle[r][c][0] ? mycelle[r][c] : " " );
			
			
			toColor(0x07);	//reset
			
			printf(" ");
		}
		printf("│");
		//END le mie barche
		//BEGIN barche avversario
		for (c=0;c<left;c++)
			printf(" ");
		
		for (c=0;c<COLNUM;c++){
			printf("│ ");
			
// 			printf("%d %d", myshots[nMyshots].x, myshots[nMyshots].y, c, r);
			
			if (myshots[nMyshots].x==c && myshots[nMyshots].y==r){
				//cursor
				
				toColor(238);
				printf("❚");
			}
			else if (hiscolpite[r][c]){
				if (hisnbInCelle[r][c] != -1){
					
					//cella è una barca colpita
					
					bool nonAffondata=false;
					for (rows=0; rows<(*hisships)[hisnbInCelle[r][c]].length; rows++){
						nonAffondata = nonAffondata || (*hisships)[hisnbInCelle[r][c]].pezzi_vivi[rows];
// 						printf("%d ", (*hisships)[hisnbInCelle[r][c]].pezzi_vivi[rows]);
					}
					
					toColor(0x0c);	//red
					if (nonAffondata)
						printf("%s",hitIcon);
					else
						printf("%s", hiscelle[r][c]);
				}
				else{
					//cella è acqua colpita
					
					toColor(147);//19
					printf("%s",waterIcon);
				}
			}
			else
				printf(" ");
			
			
			toColor(0x07);	//reset
			
			printf(" ");
		}
		printf("│");
		
		
		
		//END barche avversario
		printf("\n");
		
		if (r+1 < ROWNUM){
			//middle row
			for (rows=0; rows<2; rows++){
				
				for (c=0;c<left;c++)
					printf(" ");
				
				for (c=0;c<=COLNUM;c++)
					switch(c){
						case 0:
							printf("├───");
							break;
						case COLNUM:
							printf("┤");
							break;
						default:
							printf("┼───");
					}
			}
			printf("\n");
		}
	}
	
	for (rows=0; rows<2; rows++){
		
		for (c=0;c<left;c++)
			printf(" ");
		
		for (c=0;c<=COLNUM;c++){
			//end row
			switch(c){
				case 0:
					printf("└───");
					break;
				case COLNUM:
					printf("┘");
					break;
				default:
					printf("┴───");
			}
		}
	}
	printf("\n");
}
