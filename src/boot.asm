.code16                                                                                                                                 
.globl  _start
_start:
        # 清屏
        mov $0x03, %al
        mov $0x00, %ah
        int $0x10

        mov $0x1301, %ax     # 打印功能AH=0x13 写后光标位于串末尾AL=0x01
        mov $0x000c, %bx     # 设置打印内容的样式
        mov $msg_len, %cx    # 字符串长度 CX=10
        mov $0x0, %dx        # 设置光标起始位置
        mov $msg, %bp        # 设置打印内容的地址
        int $0x10            # 调用BIOS中断，打印字符串

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
        
msg: .ascii "Hello World"
.equ msg_len, (.-msg)


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
        # 设置内容
        mov $0x41, %al
        mov $0x0f, %ah
        # 设置DS
        mov $0x10, %ebx
        mov %ebx, %ds
        # 传输内容到显存
        mov $0xb8320, %ebx
        mov %eax, (%ebx)

        jmp .

.fill 510-(.-_start), 1, 0       # 设置魔术字符
.word 0xaa55
