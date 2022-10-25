#include "interrupt.h"
#include "io.h"

// 中断描述符的数据结构
typedef struct
{
    uint16_t offset_1;       // offset bits 0..15
    uint16_t selector;       // a code segment selector in GDT or LDT
    uint8_t zero;            // unused, set to 0
    uint8_t type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;       // offset bits 16..31
} InterruptDescriptor32;

// 初始化PIC芯片
/* https://www.cnblogs.com/flashsun/p/12334973.html */
void init_pic()
{

    /*初始化主片 */
    write_to_port(PIC_M_CTRL, 0x11); // ICW1: 边沿触发,级联8259, 需要ICW4
    write_to_port(PIC_M_DATA, 0x20); // ICW2: 起始中断向量号为0x20, 也就是IR[0-7] 为 0x20 ～ 0x27
    write_to_port(PIC_M_DATA, 0x04); // ICW3: IR2 接从片
    write_to_port(PIC_M_DATA, 0x01); // ICW4: 8086 模式, 正常EOI

    /*初始化从片 */
    write_to_port(PIC_S_CTRL, 0x11); // ICW1: 边沿触发,级联8259, 需要ICW4
    write_to_port(PIC_S_DATA, 0x28); // ICW2: 起始中断向量号为0x28, 也就是IR[8-15]为0x28 ～ 0x2F
    write_to_port(PIC_S_DATA, 0x02); // ICW3: 设置从片连接到主片的IR2 引脚
    write_to_port(PIC_S_DATA, 0x01); // ICW4: 8086 模式, 正常EOI

    // 不允许任何中断
    write_to_port(PIC_M_DATA, 0xff);
    write_to_port(PIC_S_DATA, 0xff);
}

// 初始化中断系统
void init_interrupt()
{
    // 设置IDTR寄存器的内容
    uint64_t idt_operand = ((256 * 8 - 1) | (((uint64_t)IDT_ADDR) << 16));
    asm volatile("lidt %0"
                 :
                 : "m"(idt_operand));
    // 初始化PIC
    init_pic();
    // 开中断
    asm volatile("sti");
}

// 注册中断处理函数并对PIC编程使其支持对应类型的中断
void register_ISR(uint32_t isr_addr, unsigned int irq_code)
{
    // 构造中断描述符
    InterruptDescriptor32 descriptor = {(uint16_t)isr_addr, 0x08, 0, 0b10001110, (uint16_t)(isr_addr >> 16)};
    // 将中断描述符写入特定的内存位置中，这里注意要偏移0x20，比如IRQ0是时钟中断，但是对应0x20号中断
    InterruptDescriptor32 *idt0 = (InterruptDescriptor32 *)(IDT_ADDR + (irq_code + 0x20) * 8);
    *idt0 = descriptor;

    // 设置PIC使其允许该类型中断
    uint8_t mask = read_from_port(PIC_M_DATA);
    mask = mask & ~(0x1 << irq_code);
    write_to_port(PIC_M_DATA, mask);
}

struct interrupt_frame;

// 时钟中断的全局变量
volatile uint32_t SleepCountDown = 0;

// 时钟中断服务程序
// 这里是中断服务程序在gcc下的写法
__attribute__((interrupt)) void timer_interrupt_handler(struct interrupt_frame *frame)
{
    --SleepCountDown;

    // 通知PIC中断已经被接收
    write_to_port(0x20, 0x20);
}

void set_timer_IRQ()
{
    // 设置时钟中断中断源PIT
    // 10ms: 0x2e9c = 11932    1ms:  04A9 = 1193
    write_to_port(0x43, 0x34);
    write_to_port(0x40, 0xa9);
    write_to_port(0x40, 0x04);

    // 注册时钟中断的中断处理函数
    register_ISR((uint32_t)&timer_interrupt_handler, 0);
}

// 循环队列（存放9个按键，也就是18对扫描值）
uint8_t key_queue[19] = {0};
int tail = 0;
int head = 0;

__attribute__((interrupt)) void kbd_interrupt_handler(struct interrupt_frame *frame)
{
    uint8_t res = read_from_port(0x60);
    key_queue[tail] = res;
    // 如果队列满的话，那么出队一个元素
    if ((tail + 1) % 19 == head)
    {
        head = (head + 1) % 19;
    }
    tail = (tail + 1) % 19;
    write_to_port(0x20, 0x20);
}

void set_kbd_IRQ()
{
    register_ISR((uint32_t)&kbd_interrupt_handler, 1);
}