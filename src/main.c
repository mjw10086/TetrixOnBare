#include "lib.h"
#include "interrupt.h"
#include "tetris/game.h"

int main(int argc, char const *argv[])
{
    init_interrupt();
    set_timer_IRQ();
    set_kbd_IRQ();

    game_run();

    while (1)
    {
    }
    return 0;
}
