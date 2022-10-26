.code16                                                                                                                                 
.globl  _start
_start:
        # 清屏
        mov $0x03, %al
        mov $0x00, %ah
        int $0x10

        # 设置VGA图像模式
        mov $0x13, %ax
        int $0x10

        cli                  # 切换保护模式期间关中断
        # 设置GDTR寄存器
        lgdt gdt_48

        # 开启A20
        in $0x92, %al
        or $2, %al
        out %al, $0x92

        # 设置CR0
        mov %cr0, %eax
        or $0x0001, %eax
        mov %eax, %cr0

        # 长跳转刷新指令队列
        ljmp $0x8, $start_protected_mode

        hlt

# 参考自https://en.wikipedia.org/wiki/Global_Descriptor_Table#GDT_example
gdt:
# offset 0x0 (0 bytes)
.null_descriptor:
        .quad 0x0
    
# offset 0x8 (8 bytes)
.code_descriptor:           # cs should point to this descriptor
        .word 0xffff        # segment limit first 0-15 bits
        .word 0x0           # base first 0-15 bits
        .byte 0x0           # base 16-23 bits
        .byte 0x9a          # access byte
        .byte 0b11001111    # high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
        .byte 0x0           # base 24-31 bits
    
# offset 0x10 (16 bytes)
.data_descriptor:           # ds, es, fs, gs, and ss should point to this descriptor
        .word 0xffff        # segment limit first 0-15 bits
        .word 0x0           # base first 0-15 bits
        .byte 0x0           # base 16-23 bits
        .byte 0x92          # access byte
        .byte 0b11001111    # high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
        .byte 0x0           # base 24-31 bits    

# GDTR寄存器的内容
gdt_48: 
        .word 23
        .word gdt, 0


# 通过长跳转，程序执行到这个函数时，表示已经进入保护模式
.code32
start_protected_mode:
        # 设置段寄存器
        mov $0x10, %ebx
        mov %ebx, %ds
        mov %ebx, %ss
        mov %ebx, %es
        mov %ebx, %fs
        mov %ebx, %gs

        # 设置栈起始位置
        mov $0x12000, %eax
        mov %eax, %esp
        mov %eax, %ebp

        # 读取硬盘中C语言程序的内容到内存
        mov $0x1, %eax                 # 0x1号扇区
        mov $0x20000, %ebx             # 读到0x20000内存处
        mov $27, %ecx                  # 读取27个扇区
        call rd_disk_m_32

        # 将执行权交由C语言程序
        ljmp $0x8, $0x21000


# 读取硬盘内容到内存
# 代码摘自 https://www.cnblogs.com/flashsun/p/12235761.html
rd_disk_m_32:

    mov  %eax, %esi
    mov  %cx, %di

    mov  $0x1f2, %dx
    mov  %cl, %al
    out  %al, %dx

    mov  %esi, %eax
    # 保存LBA地址
    mov  $0x1f3, %dx
    out  %al, %dx

    mov  $8, %cl
    shr  %cl, %eax
    mov  $0x1f4, %dx
    out  %al, %dx

    shr  %cl, %eax
    mov  $0x1f5, %dx
    out  %al, %dx

    shr  %cl, %eax
    and  $0x0f, %al
    or   $0xe0,%al
    mov  $0x1f6, %dx
    out  %al, %dx

    mov  $0x1f7, %dx
    mov  $0x20, %al
    out  %al, %dx

.not_ready:
    nop
    in  %dx, %al
    and $0x88, %al
    cmp $0x08, %al
    jnz .not_ready

    mov %di, %ax
    mov $256, %dx
    mul %dx
    mov %ax, %cx
    mov $0x1f0, %dx

.go_on_read:
    in  %dx, %ax
    mov %ax, %ds:(%ebx)
    add $2, %ebx
    loop .go_on_read
    ret

.fill 510-(.-_start), 1, 0       # 设置魔术字符
.word 0xaa55
