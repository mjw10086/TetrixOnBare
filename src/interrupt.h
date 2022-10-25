#ifndef __KERNEL_INTERRUPT_H_
#define __KERNEL_INTERRUPT_H_

#include <stdint.h>

#define IDT_ADDR 0x6000 // IDT存放的内存地址
#define PIC_M_CTRL 0x20 //主片控制端口
#define PIC_M_DATA 0x21 //主片数据端口
#define PIC_S_CTRL 0xa0 //从片控制端口
#define PIC_S_DATA 0xa1 //从片数据端口

// 初始化中断系统
void init_interrupt();

// 注册中断处理函数并对PIC编程使其支持对应类型的中断
void register_ISR(uint32_t isr_addr, unsigned int irq_code);

// 设置时钟中断
void set_timer_IRQ();

// 设置键盘中断
void set_kbd_IRQ();

#endif