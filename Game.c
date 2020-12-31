#include "Game.h"
#include "fxlib.h"
#include "digits.h"
#include "Graphics.h"
#include <stdio.h>
#include "Menu.h"

#define FIELD_MINE (1<<0)
#define FIELD_COVERED (1<<1)
#define FIELD_FLAGGED (1<<2)
#define FIELD_MINES_1 (1<<3)
#define FIELD_MINES_2 (1<<4)
#define FIELD_MINES_3 (1<<5)

int indexOf(int x, int y)
{
    return y * FWIDTH + x;
}

uchar _isFlagged(int x, int y, uchar *field)
{
    int i = indexOf(x,y);
    return field[i] & FIELD_FLAGGED;
}

uchar _isCovered(int x, int y, uchar *field)
{
    int i = indexOf(x,y);
    return field[i] & FIELD_COVERED;
}

uchar _isMined(int x, int y, uchar *field)
{
    int i = indexOf(x,y);
    if(x < 0 || y < 0 || x >= FWIDTH || y >= FHEIGHT) return FALSE;
    return field[i] & FIELD_MINE;
}

void _setFlagged(int x, int y, uchar *field, uchar flagged)
{
    int i = indexOf(x,y);
    if(flagged)
        field[i] |= FIELD_FLAGGED;
    else
        field[i] &= ~FIELD_FLAGGED;
}

uchar _getNaigbouringMines(uchar* field, uchar x, uchar y)
{
    uchar i = indexOf(x,y);
    return (field[i] >> 3) & 7;//0b111;
}

void _setNaigbouringMines(uchar* field, uchar x, uchar y, uchar val)
{
    uchar i = indexOf(x,y);
    uchar v = (val & 7) << 3;
    field[i] &= ~56;//0b111000;
    field[i] |= v;
}

uchar _uncover(uchar* field, uchar x, uchar y)
{
    uchar mineCount;
    int i = indexOf(x, y);

    if(x < 0 || y < 0 || x >= FWIDTH || y >= FHEIGHT) return 0;
    if(!_isCovered(x, y, field)) return 0;
    if(_isFlagged(x, y, field)) return 0;

    if(field[i] & FIELD_MINE)
    {
        return 1;
    }

    field[i] &= ~ FIELD_COVERED;

    mineCount = _getNaigbouringMines(field, x, y);
    if(mineCount == 0)
    {
        _uncover(field, x-1,y-1);
        _uncover(field, x+1,y-1);
        _uncover(field, x-1,y+1);
        _uncover(field, x+1,y+1);
        _uncover(field, x-1,y);
        _uncover(field, x+1,y);
        _uncover(field, x,y-1);
        _uncover(field, x,y+1);
    }

    return 0;
}

void _drawField(uchar *field, int selectionX, int selectionY)
{
    int i, j, k;
    uchar *digit;
    DISPBOX dispbox;

    for(i = 0; i < FWIDTH; i++)
    {
        for(j = 0; j < FHEIGHT; j++)
        {
            dispbox.left = i * TSIZE;
            dispbox.top = j * TSIZE;
            dispbox.right = dispbox.left + TSIZE;
            dispbox.bottom = dispbox.top + TSIZE;
            //Bdisp_AreaClr_DDVRAM(&dispbox);
            FillRect(i*TSIZE,j*TSIZE,TSIZE,TSIZE,0);
            SetPoint(i * TSIZE, j * TSIZE);
            if(_isCovered(i, j, field))
            {
                if(_isFlagged(i, j, field))
                {
                    digit = GetDigit(ICON_FLAG);
                    DrawDigit(i*TSIZE,j*TSIZE,digit);
                }
                else
                {
                    for(k = 0; k < (TSIZE*TSIZE); k++)
                    {
                        int x = k%TSIZE;
                        int y = k/TSIZE;
                        if(x%2 == y%2)
                            SetPoint(i*TSIZE+x, j*TSIZE+y);
                    }
                }
            }
            else
            {
                if(_isMined(i,j,field))
                {
                    digit = GetDigit(ICON_MINE);
                    DrawDigit(i*TSIZE,j*TSIZE,digit);
                }
                else
                {
                    uchar num = _getNaigbouringMines(field, i, j);
                    if(num > 0)
                    {
                        digit = GetDigit(num);
                        DrawDigit(i*TSIZE,j*TSIZE,digit);
                    }
                }
                
            }
        }
    }

    DrawRect(selectionX * TSIZE, selectionY * TSIZE, TSIZE, TSIZE);
}

void _prepareField(uchar* field)
{
    int i;
    for(i = 0; i < (FWIDTH * FHEIGHT); i++)
    {
        field[i] = FIELD_COVERED;
    }
}

void _placeMines(uchar* field, uchar freeX, uchar freeY, int mineCount)
{
    int i, ind;
    uchar x,y;

    for(i = 0; i < mineCount;)
    {
        x = rand() % FWIDTH;
        y = rand() % FHEIGHT;
        ind = indexOf(x, y);
        if((x != freeX || y != freeY) && !_isMined(x, y, field))
        {
            field[ind] |= FIELD_MINE;
            i++;
        }
    }

    for(x = 0; x < FWIDTH; x++)
    {
        for(y = 0; y < FHEIGHT; y++)
        {
            uchar val = 0;
            if(_isMined(x-1,y-1,field)) val++;
            if(_isMined(x+1,y-1,field)) val++;
            if(_isMined(x-1,y+1,field)) val++;
            if(_isMined(x+1,y+1,field)) val++;
            if(_isMined(x-1,y,field)) val++;
            if(_isMined(x+1,y,field)) val++;
            if(_isMined(x,y-1,field)) val++;
            if(_isMined(x,y+1,field)) val++;
            _setNaigbouringMines(field, x, y, val);
        }
    }
}

void _toggleFlag(uchar *field, uchar x, uchar y)
{
    if(!_isCovered(x, y, field)) return;
    if(_isFlagged(x, y, field))
        _setFlagged(x, y, field, 0);
    else
        _setFlagged(x, y, field, 1);
    
}

void _uncoverAll(uchar* field)
{
    int i;
    for(i = 0; i < FWIDTH * FHEIGHT; i++)
    {
        field[i] &= ~ FIELD_COVERED;
    }
}

uchar _isCleared(uchar *field)
{
    int i;

    uchar allFlagged = TRUE;
    uchar allUncovered = TRUE;

    for(i = 0; i < (FWIDTH*FHEIGHT); i++)
    {
        if((field[i] & FIELD_COVERED) && !(field[i] & FIELD_MINE))
        {
            allUncovered = FALSE;
        }
        if((field[i] & FIELD_MINE) && !(field[i] & FIELD_FLAGGED)
            || !(field[i] & FIELD_MINE) && (field[i] & FIELD_FLAGGED))
        {
            allFlagged = FALSE;
        }
    }

    return allFlagged || allUncovered;
}

void _flagAll(uchar *field)
{
    int i;
    for(i = 0; i < FWIDTH * FHEIGHT; i++)
    {
        if(field[i] & FIELD_MINE)
            field[i] = FIELD_MINE | FIELD_FLAGGED | FIELD_COVERED;
        else
            field[i] &= ~(FIELD_COVERED | FIELD_FLAGGED);
    }
}

uchar RunGame(uchar numMines)
{
    unsigned int key;
    uchar field[FWIDTH * FHEIGHT];
    int selX = 0, selY = 0;
    int i, j;
    uchar prepared = FALSE;
    char custText[20];
    uchar* digit;

    Clear();
    _prepareField(field);

    while(1)
    {
        uchar gameover = FALSE;

        _drawField(field, selX, selY);
        Bdisp_PutDisp_DD();
        GetKey(&key);

        switch(key)
        {
            case INP_UP:
                selY--;
                break;
            case INP_DOWN:
                selY++;
                break;
            case INP_LEFT:
                selX--;
                break;
            case INP_RIGHT:
                selX++;
                break;
            case INP_UNCOVER:
                if(!prepared)
                {
                    prepared = TRUE;
                    _placeMines(field, selX, selY, numMines);
                }
                gameover = _uncover(field, selX, selY);
                break;
            case INP_FLAG:
                _toggleFlag(field, selX, selY);
                break;
            case INP_MENU:
                PopUpWin(5);
                locate(0,5);
                for(i = 0, j = 0; i < (FWIDTH*FHEIGHT); i++)
                    if(field[i] & FIELD_FLAGGED)
                        j++;
                locate(5,5);
                sprintf(custText, ": %i", j);
                Print((uchar*)custText);
                digit = GetDigit(ICON_FLAG2);
                DrawDigit(6*3, 8*4, digit);
                locate(5,6);
                sprintf(custText, ": %i",(int)numMines);
                Print((uchar*)custText);
                digit = GetDigit(ICON_MINE);
                DrawDigit(6*3, 8*5, digit);
                if(ShowMenuNoBorder(1, "Continue\nExit Game") == 1) return GAMEND_USEREXIT;
                break;
        }
        if(selX < 0) selX = FWIDTH - 1;
        if(selY < 0) selY = FHEIGHT - 1;
        if(selX >= FWIDTH) selX = 0;
        if(selY >= FHEIGHT) selY = 0;

        if(gameover)
        {
            Clear();
            _uncoverAll(field);
            _drawField(field, -1, -1);
            Sleep(2000);
            return GAMEND_LOOSE;
        }

        if(prepared && _isCleared(field))
        {
            Clear();
            _flagAll(field);
            _drawField(field, -1, -1);
            Sleep(2000);
            return GAMEND_WIN;
        }
    }
}