#ifndef __TETROMINOES_H__
#define __TETROMINOES_H__

/* 方块种类 */
enum TETROMINOES_TYPE
{
    TYPE_I,
    TYPE_J,
    TYPE_L,
    TYPE_O,
    TYPE_S,
    TYPE_T,
    TYPE_Z
};

typedef enum TETROMINOES_TYPE TETROMINOES_TYPE;

enum TETROMINOES_TYPE get_random_tetrominoes_type();

/* 方块结构体 */
typedef struct
{
    int m_shape[4][2];
    TETROMINOES_TYPE type;
    // 记录方块的旋转状态
    int status;
} Tetrominoes;

/* 根据类型创建方块 */
Tetrominoes create_tetrominoes(TETROMINOES_TYPE type);

/* 下落 */
void drop(Tetrominoes *tetrominoes);

/* 左右移动， direction 0为左 1位右 */
void move(Tetrominoes *tetrominoes, int direction);

/* 翻转 */
void filp(Tetrominoes *tetrominoes);

/* 逆向翻转 */
void unfilp(Tetrominoes *tetrominoes);

#endif