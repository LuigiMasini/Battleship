#include <port.h>
#include <cose.h>
#include <stdio.h>

#define NUM_BARCHE 10
// #define ROWNUM 12
// #define COLNUM 12
// #define CELL_SIZE 4

barca_t barche[NUM_BARCHE];

extern const char *grid[];

#ifndef WINDOWS
// 	#include <unistd.h>
// 	#include <curses.h>
#endif

int main(){
	setUp();
	setupBarche(&barche, NUM_BARCHE);
	
	barche[0].length=5;
	barche[0].pezzi_colpiti[2] = true;
	barche[1].length=4;
	barche[2].length=3;
	
	
	renderShip(&barche[0]);
	/*
	for (int i=0; i<5; i++){
		printf("%s", barche[0].renderedShip[i]);
	}*/
	
	int currentBarca = 0, nBarcheOk=1;
	while (1){
		switch (getch()) {
			case '\033':
				// if the first value is esc
				getch(); // skip the [
				switch(getch()) { // the real value
					case 'A':
						// code for arrow up
						if (barche[currentBarca].y > 0)
							barche[currentBarca].y--;
						else 
							barche[currentBarca].y = ROWNUM-1;
						break;
					case 'B':
						// code for arrow down
						if (barche[currentBarca].y < ROWNUM-1)
							barche[currentBarca].y++;
						else
							barche[currentBarca].y = 0;
						break;
					case 'C':
						// code for arrow right
						if (barche[currentBarca].x < COLNUM-1)
							barche[currentBarca].x++;
						else
							barche[currentBarca].x = 0;
						break;
					case 'D':
						// code for arrow left
						if (barche[currentBarca].x > 0)
							barche[currentBarca].x--;
						else
							barche[currentBarca].x = COLNUM-1;
						break;
				}
				break;
			case 'R':
			case 'r':
				barche[currentBarca].direction == 3 ? barche[currentBarca].direction = 0 : barche[currentBarca].direction++;
				break;
			case '\n':
				if (barche[currentBarca].isPosValid){
					nBarcheOk++;
					currentBarca++;
				}
				else{
					toColor(0x0c);
					printf("Invalid Position for current ship: move it somewere else");
					toColor(0x07);
				}
				
		}
		printShips(&barche, nBarcheOk);
	}
}
