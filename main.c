//
//  main.c
//  SpriteGenerator
//
//  Created by Sherlock Yiu on 19/4/2017.
//  Copyright © 2017 ddd. All rights reserved.
//

#include <stdio.h>
#include "ncurses.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool quitGame = FALSE;
bool savingMode = FALSE;
bool extendedMode = FALSE;
int extendedChar = 0;

int currColor = 35;
int currX = 0;
int currY = 2;

char display[22][80];
char color[22][80];
int cx = -1, cy = -1;
char filename[50];
int filenamePos = 0;
int imageId = 0;

MEVENT event;
FILE *output;

char getColorCode(int colorPair) {
	switch (colorPair) {
		case 20:
			return 'k';
		case 21:
			return 'r';
		case 22:
			return 'g';
		case 23:
			return 'y';
		case 24:
			return 'b';
		case 25:
			return 'm';
		case 26:
			return 'c';
		case 27:
			return 'w';
		case 28:
			return 'K';
		case 29:
			return 'R';
		case 30:
			return 'G';
		case 31:
			return 'Y';
		case 32:
			return 'B';
		case 33:
			return 'M';
		case 34:
			return 'C';
		case 35:
			return 'W';
		default:
			return '\0';
	}
}

void resetSavingMode() {
	savingMode = FALSE;
	filenamePos = 0;
	for (int i=0; i<50; i++) {
		filename[i] = '\0';
	}
	mvprintw(24, 6, "                                                                                   ");
	attroff(COLOR_PAIR(35));
	attron(COLOR_PAIR(currColor));
	move(currY, currX);
}

void saveFile() {
	//char temp[100] = "/Users/sheryiu/Desktop/";
	char temp[100] = "./";
	strcat(temp, filename);
	output = fopen(temp, "a+");
	
	int top = -1;
	for (int i=0; i<22 && top == -1; i++) {
		bool allZero = TRUE;
		for (int j=0; j<80; j++) {
			if (display[i][j] != ' ') allZero = FALSE;
		}
		if (!allZero) top = i;
	}
	int height = -1;
	for (int i=21; i>=0 && height == -1; i--) {
		bool allZero = TRUE;
		for (int j=0; j<80; j++) {
			if (display[i][j] != ' ') allZero = FALSE;
		}
		if (!allZero) height = i-top+1;
	}
	int left = -1;
	for (int i=0; i<80 && left == -1; i++) {
		bool allZero = TRUE;
		for (int j=0; j<22; j++) {
			if (display[j][i] != ' ') allZero = FALSE;
		}
		if (!allZero) left = i;
	}
	int width = -1;
	for (int i=79; i>=0 && width == -1; i--) {
		bool allZero = TRUE;
		for (int j=0; j<22; j++) {
			if (display[j][i] != ' ') allZero = FALSE;
		}
		if (!allZero) width = i-left+1;
	}
	
	fprintf(output, "%d %d %d %d %d\n", imageId, width, height, cx-left, cy-top);
	for (int i=top; i<top+height; i++) {
		for (int j=left; j<left+width; j++) {
			fprintf(output, "%c", display[i][j]);
		}
		fprintf(output, "\n");
	}
	for (int i=top; i<top+height; i++) {
		for (int j=left; j<left+width; j++) {
			fprintf(output, "%c", color[i][j]);
		}
		fprintf(output, "\n");
	}
	fclose(output);
}


void frame() {
	
	while(!quitGame) {
		int ch = getch();
		switch (ch) {
			case KEY_MOUSE:
				if (getmouse(&event) == OK) {
					if (event.y >= 2 && event.y <= 23) {
						currX = event.x;
						currY = event.y;
						move(currY, currX);
						if (savingMode) {
							cx = currX;
							cy = currY-2;
							saveFile();
							resetSavingMode();
						}
					} else if (event.y == 0) {
						attroff(COLOR_PAIR(currColor));
						currColor = 20+floor(event.x/3);
						attron(COLOR_PAIR(currColor));
					} else if (event.y == 1) {
						if (event.x == 73) {
							if (imageId > 0) imageId--;
						} else if (event.x == 78) {
							imageId++;
						}
						attroff(COLOR_PAIR(currColor));
						attron(COLOR_PAIR(35));
						mvprintw(1, 75, "%2d", imageId);
						attroff(COLOR_PAIR(35));
						attron(COLOR_PAIR(currColor));
						move(currY, currX);
					} else if (event.y == 24) {
						if (!savingMode) {
							savingMode = TRUE;
							attroff(COLOR_PAIR(currColor));
							attron(COLOR_PAIR(35));
							mvprintw(24, 6, "Filename:");
							currX = 15;
							currY = event.y;
						}
					}
				}
				break;
			case KEY_F(1):
				extendedMode = !extendedMode;
				attroff(COLOR_PAIR(currColor));
				attron(COLOR_PAIR(35));
				if (extendedMode)
					mvprintw(1, 54, "On ");
				else
					mvprintw(1, 54, "Off");
				extendedChar = 0;
				attroff(COLOR_PAIR(35));
				attron(COLOR_PAIR(currColor));
				move(currY, currX);
			default:
				if (ch >= 32 && ch <= 126) {
					if (extendedMode && ch >= '0' && ch <= '9') {
						extendedChar *= 10;
						extendedChar += ch - '0';
						if ((extendedChar >= 102 && extendedChar <= 255) || extendedChar == 96 || extendedChar == 97) {
							currX++;
							addch(extendedChar | A_ALTCHARSET);
							if (extendedChar >= 161)
								display[currY-2][currX] = extendedChar;
							else if (extendedChar >= 102)
								display[currY-2][currX] = extendedChar + (128-102+2);
							else // 96 97
								display[currY-2][currX] = extendedChar + (128-96);
							color[currY-2][currX] = getColorCode(currColor);
							extendedChar = 0;
						}
					} else {
						currX++;
						addch(ch);
						if (savingMode) {
							filename[filenamePos] = ch;
							filenamePos++;
						} else {
							display[currY-2][currX] = ch;
							color[currY-2][currX] = getColorCode(currColor);
						}
					}
				}
				break;
		}
		
		refresh();
		usleep(20000);
	}
}


int main() {
	initscr();
	raw();
	keypad(stdscr, TRUE);
	timeout(0);
	noecho();
	curs_set(0);
	mousemask(ALL_MOUSE_EVENTS, NULL);
	
	start_color();
	// text color, bg color
	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_YELLOW);
	init_pair(4, COLOR_BLACK, COLOR_BLUE);
	init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(6, COLOR_BLACK, COLOR_CYAN);
	init_pair(7, COLOR_BLACK, COLOR_WHITE);
	init_pair(8, COLOR_BLACK, 8);
	init_pair(9, COLOR_BLACK, 9);
	init_pair(10, COLOR_BLACK, 10);
	init_pair(11, COLOR_BLACK, 11);
	init_pair(12, COLOR_BLACK, 12);
	init_pair(13, COLOR_BLACK, 13);
	init_pair(14, COLOR_BLACK, 14);
	init_pair(15, COLOR_BLACK, 15);
	for (int i=0; i<16; i++) {
		init_pair(20+i, i, COLOR_BLACK);
	}
	
	
	for (int i=0; i<=15; i++) {
		attron(COLOR_PAIR(currColor));
		mvprintw(0, (i<10)?i*3+1:i*3, "%d", i);
		mvprintw(1, i*3, "-");
		mvprintw(1, i*3+1, "-");
		mvprintw(1, i*3+2, "-");
		attroff(COLOR_PAIR(currColor));
		attron(COLOR_PAIR(i));
		mvprintw(0, i*3+2, " ");
		attroff(COLOR_PAIR(i));
	}
	
	attron(COLOR_PAIR(currColor));
	mvprintw(1, 35, "[F1] Extended Mode Off");
	
	mvprintw(1, 63, "Image ID: -");
	mvprintw(1, 76, "0");
	mvprintw(1, 78, "+");
	
	
	for (int i=0; i<22; i++) {
		for (int j=0; j<80; j++) {
			display[i][j] = ' ';
			color[i][j] = ' ';
		}
	}
	
	mvprintw(24, 0, "Save");
	for (int i=0; i<50; i++) {
		filename[i] = '\0';
	}
	
	move(currY, currX);
	
	while(1) {
		if (!quitGame) frame();
		else {endwin();exit(0);}
	}
	
	return 0;
}
