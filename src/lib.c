#include "lib.h"
#include "interrupt.h"

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
            else
                *p = 0xff;
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

void sleep(unsigned int millisecond)
{
    SleepCountDown = millisecond;

    while (SleepCountDown)
    {
    }
}

uint8_t read_key()
{
    // 如果队列为空则返回0
    if (tail == head)
    {
        return 0;
    }
    else
    {
        uint8_t key = key_queue[head];
        head = (head + 1) % 19;
        return key;
    }
}