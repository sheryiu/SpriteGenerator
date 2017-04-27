//
//  color.c
//  SpriteGenerator
//
//  Created by Sherlock Yiu on 27/4/2017.
//  Copyright © 2017 ddd. All rights reserved.
//

#include "color.h"

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

void initColors() {
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
}
