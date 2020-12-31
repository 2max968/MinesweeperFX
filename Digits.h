#ifndef DIGITS_H_
#define DIGITS_H_

#include "Mines.h"

#define ICON_FLAG 10
#define ICON_MINE 11
#define ICON_FLAG2 12

uchar *GetDigit(int ind);
void DrawDigit(int x, int y, uchar *digit);

#endif