#include <cose.h>
#include <port.h>
#include <stdio.h>

#define NUM_BARCHE 10

barca_t barche[NUM_BARCHE];

extern const char *grid[];

#ifndef WINDOWS
// 	#include <unistd.h>
// 	#include <curses.h>
#endif

void main(){
	setUp();
	setupBarche(&barche, NUM_BARCHE);
	
	barche[0].length=5;
	barche[0].pezzi_colpiti[2] = true;
	
	
	renderShip(&barche[0]);
	/*
	for (int i=0; i<5; i++){
		printf("%s", barche[0].renderedShip[i]);
	}*/
	
	char currentBarca = 0, nBarcheOk=1;
	while (1){
		switch (getch()) {
			case '\033':
				// if the first value is esc
				getch(); // skip the [
				switch(getch()) { // the real value
					case 'A':
						// code for arrow up
						barche[currentBarca].y--;
						break;
					case 'B':
						// code for arrow down
						barche[currentBarca].y++;
						break;
					case 'C':
						// code for arrow right
						barche[currentBarca].x++;
						break;
					case 'D':
						// code for arrow left
						barche[currentBarca].x--;
						break;
				}
				break;
			case 'R':
			case 'r':
				barche[currentBarca].direction == 3 ? barche[currentBarca].direction = 0 : barche[currentBarca].direction++;
				break;
				
		}
		printShips(&barche, nBarcheOk);
	}
}
