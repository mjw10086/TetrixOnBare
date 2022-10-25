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

extern volatile uint32_t SleepCountDown;
/* 睡眠 */
void sleep(unsigned int millisecond);

extern uint8_t key_queue[19];
extern int tail;
extern int head;

/* 读取键盘的扫描码 */
uint8_t read_key();

#endif