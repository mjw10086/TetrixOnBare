#include "lib.h"
#include "interrupt.h"

int main(int argc, char const *argv[])
{
    init_interrupt();
    set_timer_IRQ();

    char *msg = "hello world";
    for (int i = 0; i < 11; i++)
    {
        print_char(msg[i], 1, i);
        sleep(20000);
    }

    while (1)
    {
    }
    return 0;
}
