#include <port.h>
#include <cose.h>
#include <stdio.h>

barca_t myShips[NUM_BARCHE];
barca_t hisShips[NUM_BARCHE];

extern const char *grid[];
/*
#ifndef WINDOWS
	#include <unistd.h>
	#include <sys/ioctl.h>
// 	#include <curses.h>
#else
	#include <windows.h>
#endif
*/
int printMenu(bool);

int main(){
	setUp();
	
	while(!printMenu(false)){
		
		gameStart:
		setupBarche(&myShips, NUM_BARCHE);
		setupBarche(&hisShips, NUM_BARCHE);
		
		int currentBarca = 0, nBarcheOk=1, i, b;
		int columns, rows;
		
		char vince = -1;
		bool foundPezzoVivo=true;
		
		getTerminalSize(&columns, &rows);
		
		int 	top	= (rows - ROWNUM*2 - 1)/2,
			left	= (columns - COLNUM*4 - 1)/2;
			
		
		//posiziona le tue Ships
		while (nBarcheOk <= NUM_BARCHE){
			
			CLEAR;
			
			for (i=0; i<top-4; i++)
				printf("\n");
			
			for (b=0;b<nBarcheOk;b++)
				for (i=0;i<myShips[b].length;i++)
					myShips[b].pezzi_vivi[i] = b == currentBarca ? true : false;
			
			printShips(&myShips, nBarcheOk, left);
			
			printf("\n");
			
			if (!myShips[currentBarca].isPosValid){
				toColor( 0x0c );
				centerText("Invalid Position for current ship: move it somewere else");
			}
			else{
				toColor( 0x02 );
				centerText("Posizione ok");
			}
			
			printf("\n\n");
			
			toColor( 0x07 );
			
			for (i=0; i<left;i++)
				printf(" ");
			
			toColor( 0x0e );
			#ifndef WINDOWS
			printf("\e[4mfreccie\e[0;32m");
			#else
			printf("r");
			#endif
			toColor( 0x07 );
			
			printf("\tsposta la barca in quella direzione\n");
			
			for (i=0; i<left;i++)
				printf(" ");
			
			toColor( 0x0e );
			#ifndef WINDOWS
			printf("\e[4mr\e[0;32m");
			#else
			printf("r");
			#endif
			toColor( 0x07 );
			printf("\t\tper ruotare la nave\n");
			
			for (i=0; i<left;i++)
				printf(" ");
			
			toColor( 0x0e );
			#ifndef WINDOWS
			printf("\e[4menter\e[0;32m");
			#else
			printf("enter");
			#endif
			
			toColor( 0x07 );
			printf("\tper inserire la prossima nave\n");
			
			toColor(0x07);
			
			switch (getch()) {
				case '\033':
					// if the first value is esc
					getch(); // skip the [
					switch(getch()) { // the real value
						case 'A':
							// code for arrow up
							if (myShips[currentBarca].y > 0)
								myShips[currentBarca].y--;
							else 
								myShips[currentBarca].y = ROWNUM-1;
							break;
						case 'B':
							// code for arrow down
							if (myShips[currentBarca].y < ROWNUM-1)
								myShips[currentBarca].y++;
							else
								myShips[currentBarca].y = 0;
							break;
						case 'C':
							// code for arrow right
							if (myShips[currentBarca].x < COLNUM-1)
								myShips[currentBarca].x++;
							else
								myShips[currentBarca].x = 0;
							break;
						case 'D':
							// code for arrow left
							if (myShips[currentBarca].x > 0)
								myShips[currentBarca].x--;
							else
								myShips[currentBarca].x = COLNUM-1;
							break;
					}
					break;
				case 'R':
				case 'r':
					myShips[currentBarca].direction == 3 ? myShips[currentBarca].direction = 0 : myShips[currentBarca].direction++;
					break;
				case '\n':
					if (myShips[currentBarca].isPosValid){
						nBarcheOk++;
						currentBarca++;
					}
					
			}
		}
		
		//tutte le mie barche partono attive
		for (b=0;b<NUM_BARCHE;b++)
			for (i=0; i<5; i++)
				myShips[b].pezzi_vivi[i]=true;
		
		//inizializza barche avversario
		for (b=0;b<NUM_BARCHE;b++){
			do {
				hisShips[b].x=randomRange(0, ROWNUM-1);
				hisShips[b].y=randomRange(0, COLNUM-1);
				hisShips[b].direction=randomRange(0, 3);
	// 			printf("barca %d lunga %d, x %d y %d", b, hisShips[b].length, hisShips[b].x, hisShips[b].y);
	// 			printShips(&hisShips, b+1, left);
			}
			while (checkOverlapping(&hisShips, b+1));
		}
		
		for (i=0; i<top; i++)
			printf("\n");
		
	// 	printShips(&hisShips, 10, left);
		
		CLEAR;
		/*
		toColor(10);
		centerText("INIZIO PARTITA!");
		toColor(7);
		
		sleep
		clear
		*/
		
		colpo_t myShots[COLNUM*ROWNUM], hisShots[COLNUM*ROWNUM];
		int nMyShots=0, nHisShots=0;
		
		memset(myShots, 0, COLNUM*ROWNUM*sizeof(colpo_t));
		memset(hisShots, 0, COLNUM*ROWNUM*sizeof(colpo_t));
		
		myShots[0].x=0;
		myShots[0].y=0;
		
		do{
			
			CLEAR;
			
			for (i=0; i<top-4; i++)		//stesso top di sopra perchè grid è alto uguale
				printf("\n");
			
			for (i=0;i<(columns - (COLNUM*4 - 1)*2 - 1 )/3;i++)
				printf(" ");
			printf("Il tuo campo");
			
			for (i=0;i<(columns - (COLNUM*4 - 1)*2 - 1 )/3 + COLNUM*4 + 1 - 12;i++)
				printf(" ");
			printf("Il campo dell'avversario");
			
			printCampo(&myShips, &hisShips, myShots, hisShots, nMyShots, nHisShots);
			
			printf("\n");
			
			
			//check if alredy hitted that cell
			for (i=0;i<nMyShots;i++)
				if(myShots[nMyShots].x == myShots[i].x && myShots[nMyShots].y == myShots[i].y )
					break;
			if(i!=nMyShots){
				toColor(12);
				centerText("Posizione già colpita, scegline un'altra!");
				toColor(0x07);
			}
			
			printf("\n\n");
			
			
			for (i=0; i<left;i++)
				printf(" ");
			
			toColor( 0x0e );
			#ifndef WINDOWS
			printf("\e[4mfreccie\e[0;32m");
			#else
			printf("freccie");
			#endif
			toColor( 0x07 );
			printf("\tper spostare il cursore e selezionare la cella\n");
			
			
			for (i=0; i<left;i++)
				printf(" ");
			
			toColor( 0x0e );
			#ifndef WINDOWS
			printf("\e[4menter\e[0;32m");
			#else
			printf("enter");
			#endif
			toColor( 0x07 );
			printf("\tper confermare\n");
			
			
			for (i=0; i<left;i++)
				printf(" ");
			
			toColor( 0x0e );
			#ifndef WINDOWS
			printf("\e[4mm\e[0;32m");
			#else
			printf("m");
			#endif
			toColor( 0x07 );
			printf("\t\tper aprire il menù\n");
			
			switch (getch()) {
				case '\033':
					// if the first value is esc
					getch(); // skip the [
					switch(getch()) { // the real value
						case 'A':
							// code for arrow up
							if (myShots[nMyShots].y > 0)
								myShots[nMyShots].y--;
							else 
								myShots[nMyShots].y = ROWNUM-1;
							break;
						case 'B':
							// code for arrow down
							if (myShots[nMyShots].y < ROWNUM-1)
								myShots[nMyShots].y++;
							else
								myShots[nMyShots].y = 0;
							break;
						case 'C':
							// code for arrow right
							if (myShots[nMyShots].x < COLNUM-1)
								myShots[nMyShots].x++;
							else
								myShots[nMyShots].x = 0;
							break;
						case 'D':
							// code for arrow left
							if (myShots[nMyShots].x > 0)
								myShots[nMyShots].x--;
							else
								myShots[nMyShots].x = COLNUM-1;
							break;
					}
					break;
				case 'm':
				case 'M':
					if(!printMenu(true))
						goto gameStart;
					
					break;
				case '\n':
					
					//check if alredy hitted that cell
					for (i=0;i<nMyShots;i++)
						if(myShots[nMyShots].x == myShots[i].x && myShots[nMyShots].y == myShots[i].y )
							break;
					
					if(i==nMyShots){
						nMyShots++;
						myShots[nMyShots].x=0;
						myShots[nMyShots].y=0;
						
						
						char celle[ROWNUM][COLNUM][CELL_SIZE];	//acqua mycellare
						bool colpite[ROWNUM][COLNUM];
						int nbInCelle[ROWNUM][COLNUM];
						memset(*colpite, false, ROWNUM*COLNUM*sizeof(bool));
						
						for (b=0;b<nHisShots;b++)
							colpite[hisShots[b].y][hisShots[b].x]=true;
						
						battleship2grid(&myShips, NUM_BARCHE, &celle, &colpite, &nbInCelle);
						
						
						foundPezzoVivo=false;
						for (b=0; b<NUM_BARCHE && !foundPezzoVivo; b++)
							for (i=0; i<myShips[b].length && !foundPezzoVivo; i++)
							{
	// 							printf("%d %d %d_", i, myShips[b].length, !foundPezzoVivo );
								if (myShips[b].pezzi_vivi[i]){
									foundPezzoVivo=true;
									break;
								}
								
							}
						
						if (!foundPezzoVivo)
							vince=1;
						else{
							
							hisShots[nHisShots].x=randomRange(0, ROWNUM-1);
							hisShots[nHisShots].y=randomRange(0, COLNUM-1);
							nHisShots++;
							
							memset(*colpite, false, ROWNUM*COLNUM*sizeof(bool));
							
							for (b=0;b<nMyShots;b++)
								colpite[myShots[b].y][myShots[b].x]=true;
							
							battleship2grid(&hisShips, NUM_BARCHE, &celle, &colpite, &nbInCelle);
							
							foundPezzoVivo=false;
							for (b=0; b<NUM_BARCHE && !foundPezzoVivo; b++){
	// 							printf("b:%d ", b);
								for (i=0; i<hisShips[b].length && !foundPezzoVivo; i++){
									
	// 								printf("%d %d %d_", i, myShips[b].length, !foundPezzoVivo );
									if (hisShips[b].pezzi_vivi[i]){
										foundPezzoVivo=true;
										break;
									}
								}
							}
							if(!foundPezzoVivo)
								vince=0;
						}
						
					}
			}
		}
		while(vince == -1);
		
		CLEAR;
		
		for (i=0; i<top-4; i++)		//stesso top di sopra perchè grid è alto uguale
			printf("\n");
		
		for (i=0;i<(columns - (COLNUM*4 - 1)*2 - 1 )/3;i++)
			printf(" ");
		printf("Il tuo campo");
		
		for (i=0;i<(columns - (COLNUM*4 - 1)*2 - 1 )/3 + COLNUM*4 + 1 - 12;i++)
			printf(" ");
		printf("Il campo dell'avversario");
		
		printCampo(&myShips, &hisShips, myShots, hisShots, nMyShots, nHisShots);
		
		printf("\n");
		
		
		if (vince == 1){
			
			for (i=0; i<(columns-11-1)/2;i++)
				printf(" ");
			
			toColor(12);
			printf("HAI PERSO!!");
		}
		else {
			
			for (i=0; i<(columns-11-1)/2;i++)
				printf(" ");
			
			toColor(26);
			printf("HAI VINTO!!");
		}
		
		toColor(7);
		printf("\n\n");
		
		centerText("premi un qualsiasi tasto per continuare");
		
		getch();
		
	}
}

int printMenu(bool isMiddle){
	
	int InputMenu, i=0;
	int selezione=0;
	
	int rows, cols;
	getTerminalSize(&cols, &rows);
	
	while(1)
	{
		
		CLEAR;
		 
		for (i=10;i<(rows - 10)/2;i++)
			printf("\n");
		
		toColor(7);
		centerText("LE FRAGOLINE");
		printf("\n\n\n\n\n\n");
		
		if (selezione==0) toColor (14);
		else toColor(13);
		
		centerText("nuova partita\n");
		
		if(isMiddle){
			if (selezione==1) toColor (14);
			else toColor(13);
			
			centerText("continua partita in corso\n");
		}
		
		if (selezione==2) toColor (14);
		else toColor(13);
		
		centerText("esci");
		
		toColor(7);
		
		InputMenu = getch();
		if(InputMenu=='\033')
		{
			getch();	//skip the [
			switch (getch()){
				case 'A': //su
					if (selezione==0) selezione=2;
					else if (selezione==1) selezione=0;
					else selezione = isMiddle;	//if selezione == 2
					break;
				case 'B': //giu
					if (selezione==2) selezione=0;
					else if (selezione==1) selezione=2;
					else selezione = !isMiddle + 1;	//if selezione == 0
			}
		}
		else if (InputMenu=='\n')
		{
			if (selezione == 2){
				printf("\n");
				CLEAR;
				exit(0);
			}
			return selezione;
		}
	}
	
}
