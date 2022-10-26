#ifndef __GAME_H__
#define __GAME_H__

#include "tetrominoes.h"
#include "consts.h"

void game_run();

/* 游戏的渲染函数 */
void render_backgroud();                                           // 网格
void render_block(unsigned int row, unsigned int col, char color); // 填充砖块
void render_matrix(int matrix[GAME_ROW][GAME_COL]);                // 游戏中已经积累的方块
void render_tetrominoes(Tetrominoes *t);                           // 渲染tetrominoes活动块

/* 将活动块的内容复制到积累的方块数组里，需要后续再重新生成活动块 */
void update_tetrominoes_to_matrix(int matrix[GAME_ROW][GAME_COL], Tetrominoes *t);

/* 行清理，当积累块中存在满行的时候清理积累块上该行的内容，然后返回1，后续需要手动刷新显示 */
int line_clear(int matrix[GAME_ROW][GAME_COL]);

int drop_impack_check(int matrix[GAME_ROW][GAME_COL], Tetrominoes *t);

int overlap_impack_check(int matrix[GAME_ROW][GAME_COL], Tetrominoes *t);

void clear_matrix(int matrix[GAME_ROW][GAME_COL]);

#endif