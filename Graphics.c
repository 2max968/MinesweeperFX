#include "Graphics.h"
#include "fxlib.h"

void DrawRect(int x, int y, int w, int h)
{
    int i;

    for(i = 0; i < w; i++)
    {
        Bdisp_SetPoint_VRAM(x + i, y, 1);
        Bdisp_SetPoint_VRAM(x + i, y + h, 1);
    }
    for(i = 1; i < h - 1; i++)
    {
        Bdisp_SetPoint_VRAM(x, y + i, 1);
        Bdisp_SetPoint_VRAM(x + w, y + i, 1);
    }
}

void SetPoint(int x, int y)
{
    Bdisp_SetPoint_VRAM(x, y, 1);
}

void Clear()
{
    Bdisp_AllClr_DDVRAM();
}

void FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t val)
{
    uint8_t i,j;
    for(i = 0; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            Bdisp_SetPoint_VRAM(x+i,y+j,val);
        }
    }
}

void FillDotted()
{
    int x,y;
    for(x = 0; x < 128; x++)
    {
        for(y = 0; y < 64; y++)
        {
            if((x%2)==(y%2))
                Bdisp_SetPoint_VRAM(x, y, 1);
            else
                Bdisp_SetPoint_VRAM(x, y, 0);
        }
    }
}