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
	#include <unistd.h>
	#include <sys/ioctl.h>
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
	
	int currentBarca = 0, nBarcheOk=1, i;
	while (1){
#ifdef WINDOWS
		system("cls");
#else
		system("clear");
		
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		
		int 	top	= (w.ws_row - ROWNUM*2 - 1)/2,
			left	= (w.ws_col - COLNUM*4 - 1)/2;
		for (i=0; i<top; i++)
			printf("\n");
#endif
		char message[60];
		
		
		for (int b=0;b<nBarcheOk;b++)
			for (i=0;i<barche[b].length;i++)
				barche[b].pezzi_colpiti[i] = b == currentBarca ? true : false;
		
		printShips(&barche, nBarcheOk, left);
		
		if (!barche[currentBarca].isPosValid){
			strcpy(message, "Invalid Position for current ship: move it somewere else");
			toColor( 0x0c );
		}
		else{
			strcpy(message,"Posizione ok, enter per inserire la prossima nave");
			toColor( 0x0a );
		}
		
		for (i=0; i<(w.ws_col - strlen(message))/2; i++)
			printf(" ");
	
		#ifndef WINDOWS
		if (barche[currentBarca].isPosValid)
			strcpy(message,"Posizione ok, \e[0;32;4menter\e[0;1;32m per inserire la prossima nave");
		#endif
		
		printf(message);
		toColor(0x07);
		
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
				
		}
	}
}
