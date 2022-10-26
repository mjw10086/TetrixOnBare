#include "game.h"
#include "lib.h"

void game_run()
{
    srand(58444);
    int matrix[GAME_ROW][GAME_COL] = {0};

    Tetrominoes t = create_tetrominoes(get_random_tetrominoes_type());
    render_backgroud();

    int drop_count = 0;
    int speed = 60;

    while (1)
    {
        if (drop_count == speed)
        {
            if (!drop_impack_check(matrix, &t))
            {
                drop(&t);
                render_matrix(matrix);
                render_tetrominoes(&t);
            }
            else
            {
                update_tetrominoes_to_matrix(matrix, &t);
                t = create_tetrominoes(get_random_tetrominoes_type());
                if (drop_impack_check(matrix, &t))
                {
                    sleep(20000);
                    t = create_tetrominoes(get_random_tetrominoes_type());
                    clear_matrix(matrix);
                    render_matrix(matrix);
                    render_tetrominoes(&t);
                }
                if (line_clear(matrix))
                {
                    render_matrix(matrix);
                    render_tetrominoes(&t);
                }
            }
        }
        uint8_t key = read_key();
        switch (key)
        {
        case 0x11: // w
            filp(&t);
            if (overlap_impack_check(matrix, &t))
                unfilp(&t);
            break;
        case 0x1e: // a
            move(&t, 0);
            if (overlap_impack_check(matrix, &t))
            {
                move(&t, 1);
            }
            break;
        case 0x20: // d
            move(&t, 1);
            if (overlap_impack_check(matrix, &t))
            {
                move(&t, 0);
            }
            break;
        case 0x1f: // s
            while (!drop_impack_check(matrix, &t))
            {
                drop(&t);
                sleep(700);
                render_matrix(matrix);
                render_tetrominoes(&t);
            }
            break;
        default:
            break;
        }

        if (key)
        {
            render_matrix(matrix);
            render_tetrominoes(&t);
        }

        sleep(300);
        ++drop_count;
        if (drop_count > speed)
            drop_count = 0;
    }
}

void render_backgroud()
{
    for (int i = 0; i < GAME_ROW + 1; i++)
    {
        unsigned int r = MATRIX_TOP_MARGIN + BLOCK_SIZE * i;
        draw_line(r, r, MATRIX_LEFT_MARGIN, MATRIX_LEFT_MARGIN + BLOCK_SIZE * GAME_COL, LIGHTBLUE);
    }
    for (int j = 0; j < GAME_COL + 1; j++)
    {
        unsigned int c = MATRIX_LEFT_MARGIN + 9 * j;
        draw_line(MATRIX_TOP_MARGIN, MATRIX_TOP_MARGIN + BLOCK_SIZE * GAME_ROW, c, c, LIGHTBLUE);
    }
}

void render_block(unsigned int row, unsigned int col, char color)
{
    draw_square(MATRIX_TOP_MARGIN + row * BLOCK_SIZE + 1, MATRIX_LEFT_MARGIN + col * BLOCK_SIZE + 1, BLOCK_SIZE - 1, BLOCK_SIZE - 1, color);
}

void render_matrix(int matrix[GAME_ROW][GAME_COL])
{
    for (int i = 0; i < GAME_ROW; i++)
    {
        for (int j = 0; j < GAME_COL; j++)
        {
            if (matrix[i][j])
            {
                render_block(i, j, RED);
            }
            else
            {
                render_block(i, j, BLACK);
            }
        }
    }
}

void render_tetrominoes(Tetrominoes *t)
{
    for (int i = 0; i < 4; i++)
    {
        if (t->m_shape[i][0] < 0)
            continue;
        render_block(t->m_shape[i][0], t->m_shape[i][1], RED);
    }
}

void update_tetrominoes_to_matrix(int matrix[GAME_ROW][GAME_COL], Tetrominoes *t)
{
    for (int i = 0; i < 4; i++)
    {
        if (t->m_shape[i][1] < 0)
            continue;
        matrix[t->m_shape[i][0]][t->m_shape[i][1]] = 1;
    }
}

int line_clear(int matrix[GAME_ROW][GAME_COL])
{
    int ret = 0;
    int full_row[GAME_ROW];
    for (int r = 0; r < GAME_ROW; ++r)
    {
        full_row[r] = 1;
        for (int x = 0; x < GAME_COL; ++x)
        {
            if (!matrix[r][x])
                full_row[r] = 0;
        }
        if (full_row[r])
            ret = 1;
    }

    int empty_line = 0;
    for (int r = GAME_ROW - 1; r >= 0; --r)
    {
        if (full_row[r])
            empty_line += 1;

        if (empty_line != 0)
        {
            while (r - empty_line >= 0 && full_row[r - empty_line])
            {
                empty_line += 1;
            }

            for (int x = 0; x < GAME_COL; ++x)
            {
                if (r - empty_line >= 0)
                {
                    matrix[r][x] = matrix[r - empty_line][x];
                    matrix[r - empty_line][x] = 0;
                }
                else
                {
                    matrix[r][x] = 0;
                }
            }
        }
    }

    return ret;
}

int drop_impack_check(int matrix[GAME_ROW][GAME_COL], Tetrominoes *t)
{
    for (int i = 0; i < 4; i++)
    {
        int row = t->m_shape[i][0];
        int col = t->m_shape[i][1];

        if (row < -1)
            continue;

        if (matrix[row + 1][col] || row == GAME_ROW - 1)
        {
            return 1;
        }
    }
    return 0;
}

int overlap_impack_check(int matrix[GAME_ROW][GAME_COL], Tetrominoes *t)
{
    for (int i = 0; i < 4; i++)
    {
        int row = t->m_shape[i][0];
        int col = t->m_shape[i][1];

        if (row < 0)
            continue;

        if (col < 0 || col > GAME_COL - 1)
            return 1;

        if (matrix[row][col])
            return 1;
    }
    return 0;
}

void clear_matrix(int matrix[GAME_ROW][GAME_COL])
{
    for (int i = 0; i < GAME_ROW; i++)
    {
        for (int j = 0; j < GAME_COL; j++)
        {
            matrix[i][j] = 0;
        }
    }
}