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
  
        hlt
        
msg: .ascii "Hello World"
.equ msg_len, (.-msg)
 
.fill 510-(.-_start), 1, 0       # 设置魔术字符
.word 0xaa55
