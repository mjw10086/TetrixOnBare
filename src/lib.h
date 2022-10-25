#ifndef ___KERNEL_LIB_H_
#define ___KERNEL_LIB_H_

#include <stdint.h>

/* VRAM地址与屏幕属性 */
#define VRAM_START_ADDR 0x0A0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

/* 点阵字库 */
#include "ascii_font.h"

/* 打印文字 */
void print_char(char c, unsigned int row, unsigned int col);
void print_str(char *c, unsigned int row, unsigned int col);

/* 画方块 */
void draw_square(unsigned int row, unsigned int col, unsigned int width, unsigned int height, uint8_t color);

#endif