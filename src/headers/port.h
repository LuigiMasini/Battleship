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
#else
	#include <string.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
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
			code = 43;
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
			code = 103;
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

#include <stdlib.h>

void toColor(int winColorCode){
	

	#ifndef WINDOWS
	char hexcolor[3], ansiColor[13] = "\e[";
	
	sprintf(hexcolor,"%02x", winColorCode);


	int colore=strtol(&hexcolor[1], NULL, 16);
	hexcolor[1]='\0';
	int sfondo=strtol(&hexcolor[0], NULL, 16);
	
// 	char sfondo = (winColorCode -1)/ 0x0F;
// 	char colore = winColorCode % 0x10;
	
	
	//incorretto ma comodo
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
/*ANSI color codes
 *   FG BG
 *  30 40  = Black
 *  31 41  = Red
 *  32 42  = Green
 *  33 43  = Yellow
 *  34 44  = Blue
 *  35 45  = Magenta
 *  36 46  = Cyan
 *  37 47  = White
 *  90 100 = Bright Black
 *  91 101 = Bright Red
 *  92 102 = Bright Green
 *  93 103 = Bright Yellow
 *  94 104 = Bright Blue
 *  95 105 = Bright Magenta
 *  96 106 = Bright Cyan
 *  97 107 = Bright White
 */
/*windows prompt colors (bg, fg)
 *  0 = Black
 *  1 = Blue
 *  2 = Green
 *  3 = Aqua
 *  4 = Red
 *  5 = Purple
 *  6 = Yellow
 *  7 = White
 *  8 = Gray
 *  9 = Light Blue
 *  A = Light Green
 *  B = Light Aqua
 *  C = Light Red
 *  D = Light Purple
 *  E = Light Yellow
 *  F = Bright White
 */

void getTerminalSize(int *cols, int *rows){
	#ifdef WINDOWS
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	*cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	*rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	
	#else
	
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	*cols = w.ws_col;
	*rows = w.ws_row;
	#endif
}

#include <stdlib.h>

#ifdef WINDOWS
#define CLEAR system("cls")
#else
#define CLEAR system("clear");
#endif
