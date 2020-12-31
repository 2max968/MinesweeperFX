/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/
#include "fxlib.h"
#include "Menu.h"
#include "InfoTexts.h"
#include "Game.h"
#include "Mines.h"
#include "Graphics.h"
#include <stdio.h>

#define STATE_MAINMENU 0
#define STATE_DIFFMENU 1
#define STATE_HELP 2
#define STATE_ABOUT 3
#define STATE_GAME 4
#define STATE_LOOSE 5
#define STATE_WIN 6
#define STATE_CUSTOMGAME 7

int AddIn_main(int isAppli, unsigned short OptionNum)
{
    int res;
	int state = STATE_MAINMENU;
	unsigned int key;
	int difficulty;
	uchar gameResult;

    Bdisp_AllClr_DDVRAM();

    while(1)
    {
		switch(state)
		{
		case STATE_MAINMENU:
			FillDotted();
			res = ShowMenu(0, "Start Game\nHelp\nAbout");
			if(res == 0) state = STATE_DIFFMENU;
			if(res == 1) state = STATE_HELP;
			if(res == 2) state = STATE_ABOUT;
			break;
		case STATE_DIFFMENU:
			res = ShowMenu(1, "10 - Mines\n15 - Mines\n20 - Mines\nCustom Game");
			if(res == -1) state = STATE_MAINMENU;
			else
			{
				state = STATE_GAME;
				switch(res)
				{
					case 0: difficulty = 10; break;
					case 1: difficulty = 15; break;
					case 2: difficulty = 20; break;
					case 3: state = STATE_CUSTOMGAME; break;
				}
			}
			break;
		case STATE_HELP:
			ShowText(GetTextHelp());
			state = STATE_MAINMENU;
			break;
		case STATE_ABOUT:
			ShowText(GetTextAbout());
			state = STATE_MAINMENU;
			break;
		case STATE_GAME:
			gameResult = RunGame(difficulty);
			if(gameResult == GAMEND_LOOSE)
				state = STATE_LOOSE;
			else if(gameResult == GAMEND_WIN)
				state = STATE_WIN;
			else if(gameResult == GAMEND_USEREXIT)
				state = STATE_MAINMENU;
			break;
		case STATE_LOOSE:
			ShowText(GetGameOver());
			state = STATE_MAINMENU;
			break;
		case STATE_WIN:
			ShowText(GetWinText());
			state = STATE_MAINMENU;
			break;
		case STATE_CUSTOMGAME:
			PopUpWin(5);
			locate(3,3);
			Print((uchar*)"Number of mines:");
			if(!ReadInt(&difficulty, 3, 3, 4))
			{
				state = STATE_DIFFMENU;
				break;
			}
			if(difficulty < 1)
			{
				ShowText("\nYou need at\nleast 1 mine.");
			}
			else if(difficulty > 127)
			{
				ShowText("\nYou can only\nplace 127 mines.");
			}
			else
			{
				state = STATE_GAME;
			}
			break;
		}
    }

    return 1;
}




//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

