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


void draw_line(unsigned int start_row, unsigned int end_row, unsigned int start_col, unsigned int end_col, uint8_t color)
{
    if (start_row == end_row)
    {
        char *p = (char *)(VRAM_START_ADDR + start_row * SCREEN_WIDTH + start_col);
        for (int i = start_col; i < end_col; i++)
        {
            *p = color;
            ++p;
        }
    }
    else if (start_col == end_col)
    {
        char *p = (char *)(VRAM_START_ADDR + start_row * SCREEN_WIDTH + start_col);
        for (int i = start_row; i < end_row; i++)
        {
            *p = color;
            p += SCREEN_WIDTH;
        }
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


/* 随机函数， 摘抄自标准库 */
static unsigned long int next = 1;  // NB: "unsigned long int" is assumed to be 32 bits wide
 
int rand(void)  // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int) (next / 65536) % 32768;
}
 
void srand(unsigned int seed)
{
    next = seed;
}