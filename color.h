//
//  color.h
//  SpriteGenerator
//
//  Created by Sherlock Yiu on 27/4/2017.
//  Copyright © 2017 ddd. All rights reserved.
//

#if defined(_WIN32) || defined(_WIN64)
#include "curses.h"
#include <Windows.h>
#else
#include "ncurses.h"
#include <unistd.h>
#endif

char getColorCode(int colorPair);

void initColors();
