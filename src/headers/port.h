#include <termios.h>
#include <stdio.h>

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	current = old; /* make new settings same as old settings */
	current.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo) {
		current.c_lflag |= ECHO; /* set echo mode */
	} else {
		current.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
	return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
	return getch_(1);
}
#ifdef WINDOWS
	#include <windows.h>
#endif
#ifndef WINDOWS
	#include <string.h>
#endif

#include <stdbool.h>

int code2code(int cod, bool isBack){
	
// 	printf("%d ",cod);
	
	int code = cod;
	switch (code){
		case 0x00:
			code = 40;
			break;
		case 0x01:
			code = 104;
			break;
		case 0x02:
			code = 102;
			break;
		case 0x03:
			code = 106;
			break;
		case 0x04:
			code = 101;
			break;
		case 0x05:
			code = 105;
			break;
		case 0x06:
			code = 103;
			break;
		case 0x07:
			code = 107;
			break;
		case 0x08:
			code = 100;
			break;
		case 0x09:
			code = 44;
			break;
		case 0x0a:
			code = 42;
			break;
		case 0x0b:
			code = 46;
			break;
		case 0x0c:
			code = 41;
			break;
		case 0x0d:
			code = 45;
			break;
		case 0x0e:
			code = 43;
			break;
		case 0x0f:
			code = 47;
	}
	
	if (!isBack){
		code-=10;
	}
	
// 	printf("%d ", code);
	
	return code;
}

void toColor(int winColorCode){
	
	#ifndef WINDOWS
	char ansiColor[10] = "\e[";
	
	int sfondo = (winColorCode -1)/ 0x0F;
	int colore = winColorCode % 0x10;
	
	
	if (colore>=0x08)
		strcat(ansiColor+2, "1;");	//bold
	else
		strcat(ansiColor+2, "0;");	//bold
	
	sfondo = code2code(sfondo, true);
	colore = code2code(colore, false);
	
	sprintf(ansiColor+4, "%d;%dm", sfondo, colore);
	printf("%s", ansiColor);
	
	#elif
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// 	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
// 	WORD saved_attributes;
	
	/* Save current attributes */
// 	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
// 	saved_attributes = consoleInfo.wAttributes;
	
	SetConsoleTextAttribute(hConsole, winColorCode);
	
	/* Restore original attributes */
// 	SetConsoleTextAttribute(hConsole, saved_attributes);
	#endif
	
}
