#ifndef MENU_H_
#define MENU_H_

#include "Mines.h"

int ShowMenu(char canCanel, char* options);
int ShowMenuNoBorder(char canCancel, char* options);
void ShowText(char* text);
uchar ReadInt(int *value, int digits, int column, int row);

#endif