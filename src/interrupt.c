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
    mask = mask | (0x1 << irq_code);
    write_to_port(PIC_M_DATA, mask);
}