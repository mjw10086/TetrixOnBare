#include "lib.h"

void print_char(char c, unsigned int row, unsigned int col)
{
    // TODO: 检查传入参数范围
    const unsigned char *pattern = font_char[(int)c - 0x20];

    char *p = (char *)(VRAM_START_ADDR + row * FONT_HEIGHT * SCREEN_WIDTH + col * FONT_WIDTH);
    char color = 0x0f;

    for (int i = 0; i < 16; i++)
    {
        unsigned char line = pattern[i];
        unsigned char mask = 0x80;
        for (int j = 0; j < 8; j++)
        {
            if (mask & line)
                *p = color;
            mask = mask >> 1;
            ++p;
        }
        p = p - 8 + SCREEN_WIDTH;
    }
}

void print_str(char *c, unsigned int row, unsigned int col)
{
    while (*c)
    {
        print_char(*c, row, col);
        ++col;
        if (col >= SCREEN_WIDTH / FONT_WIDTH)
        {
            ++row;
            col = 0;
        }
        ++c;
    }
}

void draw_square(unsigned int row, unsigned int col, unsigned int width, unsigned int height, uint8_t color)
{
    char *p = (char *)(VRAM_START_ADDR + row * SCREEN_WIDTH + col);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *p = color;
            ++p;
        }
        p = p + SCREEN_WIDTH - width;
    }
}