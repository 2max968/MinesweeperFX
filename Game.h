#ifndef GAME_H_
#define GAME_H_

#include "fxlib.h"
#include "Mines.h"

#define GAMEND_USEREXIT 1
#define GAMEND_LOOSE 2
#define GAMEND_WIN 3

#define TRUE 1
#define FALSE 0

#define TSIZE 8
#define FHEIGHT (64/8)
#define FWIDTH (128/8)

#define INP_LEFT KEY_CTRL_LEFT
#define INP_RIGHT KEY_CTRL_RIGHT
#define INP_UP KEY_CTRL_UP
#define INP_DOWN KEY_CTRL_DOWN
#define INP_FLAG KEY_CTRL_F1
#define INP_UNCOVER KEY_CTRL_ALPHA
#define INP_MENU KEY_CTRL_EXIT

uchar RunGame(uchar numMines);

#endif