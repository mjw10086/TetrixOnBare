#include "lib.h"
#include "interrupt.h"

int main(int argc, char const *argv[])
{
    init_interrupt();
    set_timer_IRQ();
    set_kbd_IRQ();

    while (1)
    {
        uint8_t key = read_key();

        switch (key)
        {
        case 0x11: // w
            print_char('w', 1, 1);
            break;
        case 0x1e: // a
            print_char('a', 1, 1);
            break;
        case 0x20: // d
            print_char('d', 1, 1);
            break;
        case 0x1f: // s
            print_char('s', 1, 1);
            break;
        default:
            break;
        }
    }
    return 0;
}
