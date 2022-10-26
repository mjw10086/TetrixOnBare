#include "tetrominoes.h"
#include "consts.h"
#include "lib.h"

TETROMINOES_TYPE get_random_tetrominoes_type()
{
    return (TETROMINOES_TYPE)(rand() % 7);
}

void set_tetrominoes_shape(Tetrominoes *t, int shape[4][2])
{
    for (int i = 0; i < 4; i++)
    {
        t->m_shape[i][0] = shape[i][0];
        t->m_shape[i][1] = shape[i][1];
    }
}

Tetrominoes create_tetrominoes(TETROMINOES_TYPE type)
{
    Tetrominoes t;
    t.status = 0;
    t.type = type;
    int mid_x = GAME_COL / 2;

    if (type == TYPE_I)
    {
        int shapeI[4][2] = {{
                                0,
                                mid_x - 2,
                            },
                            {
                                0,
                                mid_x - 1,
                            },
                            {
                                0,
                                mid_x,
                            },
                            {
                                0,
                                mid_x + 1,
                            }};
        set_tetrominoes_shape(&t, shapeI);
    }
    else if (type == TYPE_J)
    {
        int shapeJ[4][2] = {{
                                0,
                                mid_x - 1,
                            },
                            {
                                0,
                                mid_x - 2,
                            },
                            {
                                0,
                                mid_x,
                            },
                            {
                                -1,
                                mid_x - 2,
                            }};
        set_tetrominoes_shape(&t, shapeJ);
    }
    else if (type == TYPE_L)
    {
        int shapeL[4][2] = {{
                                0,
                                mid_x - 1,
                            },
                            {
                                0,
                                mid_x - 2,
                            },
                            {
                                0,
                                mid_x,
                            },
                            {
                                -1,
                                mid_x,
                            }};
        set_tetrominoes_shape(&t, shapeL);
    }
    else if (type == TYPE_O)
    {
        int shapeO[4][2] = {{
                                -2,
                                mid_x,
                            },
                            {
                                -2,
                                mid_x - 1,
                            },
                            {
                                -1,
                                mid_x,
                            },
                            {
                                -1,
                                mid_x - 1,
                            }};
        set_tetrominoes_shape(&t, shapeO);
    }
    else if (type == TYPE_S)
    {
        int shapeS[4][2] = {{
                                -1,
                                mid_x - 1,
                            },
                            {
                                -2,
                                mid_x,
                            },
                            {
                                -2,
                                mid_x - 1,
                            },
                            {
                                -1,
                                mid_x - 2,
                            }};
        set_tetrominoes_shape(&t, shapeS);
    }

    else if (type == TYPE_T)
    {
        int shapeT[4][2] = {{
                                -1,
                                mid_x - 1,
                            },
                            {
                                -2,
                                mid_x - 1,
                            },
                            {
                                -1,
                                mid_x - 2,
                            },
                            {
                                -1,
                                mid_x,
                            }};
        set_tetrominoes_shape(&t, shapeT);
    }
    else if (type == TYPE_Z)
    {
        int shapeZ[4][2] = {{
                                -1,
                                mid_x - 1,
                            },
                            {
                                -2,
                                mid_x - 2,
                            },
                            {
                                -2,
                                mid_x - 1,
                            },
                            {
                                -1,
                                mid_x,
                            }};
        set_tetrominoes_shape(&t, shapeZ);
    }

    return t;
}

void drop(Tetrominoes *tetrominoes)
{
    for (int i = 0; i < 4; i++)
    {
        tetrominoes->m_shape[i][0] += 1;
    }
}

void move(Tetrominoes *tetrominoes, int direction)
{
    for (int i = 0; i < 4; i++)
    {
        tetrominoes->m_shape[i][1] += direction ? 1 : -1;
    }
}

void I_filp(Tetrominoes *t)
{
    if (t->status == 0)
    {
        t->m_shape[0][1] = t->m_shape[2][1];
        t->m_shape[0][0] = t->m_shape[2][0] + 2;
        t->m_shape[1][1] = t->m_shape[2][1];
        t->m_shape[1][0] = t->m_shape[2][0] + 1;
        t->m_shape[3][1] = t->m_shape[2][1];
        t->m_shape[3][0] = t->m_shape[2][0] - 1;
        t->status = 1;
    }
    else if (t->status == 1)
    {
        t->m_shape[0][1] = t->m_shape[1][1] - 2;
        t->m_shape[0][0] = t->m_shape[1][0];
        t->m_shape[2][1] = t->m_shape[1][1] - 1;
        t->m_shape[2][0] = t->m_shape[1][0];
        t->m_shape[3][1] = t->m_shape[1][1] + 1;
        t->m_shape[3][0] = t->m_shape[1][0];
        t->status = 2;
    }
    else if (t->status == 2)
    {
        t->m_shape[0][1] = t->m_shape[2][1];
        t->m_shape[0][0] = t->m_shape[2][0] + 1;
        t->m_shape[1][1] = t->m_shape[2][1];
        t->m_shape[1][0] = t->m_shape[2][0] - 1;
        t->m_shape[3][1] = t->m_shape[2][1];
        t->m_shape[3][0] = t->m_shape[2][0] - 2;
        t->status = 3;
    }
    else if (t->status == 3)
    {
        t->m_shape[0][1] = t->m_shape[1][1] - 1;
        t->m_shape[0][0] = t->m_shape[1][0];
        t->m_shape[2][1] = t->m_shape[1][1] + 1;
        t->m_shape[2][0] = t->m_shape[1][0];
        t->m_shape[3][1] = t->m_shape[1][1] + 2;
        t->m_shape[3][0] = t->m_shape[1][0];
        t->status = 0;
    }
}

void I_unfilp(Tetrominoes *t)
{
    if (t->status == 0)
    {
        t->m_shape[0][1] = t->m_shape[1][1];
        t->m_shape[0][0] = t->m_shape[1][0] - 1;
        t->m_shape[2][1] = t->m_shape[1][1];
        t->m_shape[2][0] = t->m_shape[1][0] + 1;
        t->m_shape[3][1] = t->m_shape[1][1];
        t->m_shape[3][0] = t->m_shape[1][0] + 2;
        t->status = 3;
    }
    else if (t->status == 3)
    {
        t->m_shape[0][1] = t->m_shape[2][1] - 1;
        t->m_shape[0][0] = t->m_shape[2][0];
        t->m_shape[1][1] = t->m_shape[2][1] + 1;
        t->m_shape[1][0] = t->m_shape[2][0];
        t->m_shape[3][1] = t->m_shape[2][1] + 2;
        t->m_shape[3][0] = t->m_shape[2][0];
        t->status = 2;
    }
    else if (t->status == 2)
    {
        t->m_shape[0][1] = t->m_shape[1][1];
        t->m_shape[0][0] = t->m_shape[1][0] + 1;
        t->m_shape[2][1] = t->m_shape[1][1];
        t->m_shape[2][0] = t->m_shape[1][0] - 1;
        t->m_shape[3][1] = t->m_shape[1][1];
        t->m_shape[3][0] = t->m_shape[1][0] - 2;
        t->status = 1;
    }
    else if (t->status == 1)
    {
        t->m_shape[0][1] = t->m_shape[2][1] - 2;
        t->m_shape[0][0] = t->m_shape[2][0];
        t->m_shape[1][1] = t->m_shape[2][1] - 1;
        t->m_shape[1][0] = t->m_shape[2][0];
        t->m_shape[3][1] = t->m_shape[2][1] + 1;
        t->m_shape[3][0] = t->m_shape[2][0];
        t->status = 0;
    }
}

void __filp(Tetrominoes *t)
{
    int middle[2] = {t->m_shape[0][0], t->m_shape[0][1]};

    for (int i = 1; i < 4; i++)
    {
        int row_offset = t->m_shape[i][0] - middle[0];
        int col_offset = t->m_shape[i][1] - middle[1];

        t->m_shape[i][0] = middle[0] + col_offset;
        t->m_shape[i][1] = middle[1] - row_offset;
    }
}

void __unfilp(Tetrominoes *t)
{
    int middle[2] = {t->m_shape[0][0], t->m_shape[0][1]};

    for (int i = 1; i < 4; i++)
    {
        int row_offset = t->m_shape[i][0] - middle[0];
        int col_offset = t->m_shape[i][1] - middle[1];

        t->m_shape[i][0] = middle[0] - col_offset;
        t->m_shape[i][1] = middle[1] + row_offset;
    }
}

void filp(Tetrominoes *tetrominoes)
{
    switch (tetrominoes->type)
    {
    case TYPE_I:
        I_filp(tetrominoes);
        break;
    case TYPE_O:
        break;
    default:
        __filp(tetrominoes);
    }
}

void unfilp(Tetrominoes *tetrominoes)
{
    switch (tetrominoes->type)
    {
    case TYPE_I:
        I_unfilp(tetrominoes);
        break;
    case TYPE_O:
        break;
    default:
        __unfilp(tetrominoes);
    }
}