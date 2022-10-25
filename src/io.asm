.code32
.globl write_to_port, read_from_port
.type write_to_port, @function
.type read_from_port, @function

write_to_port:
    push %edx
    push %eax
    mov 12(%esp), %edx   # port
    mov 16(%esp), %eax   # value
    out %al, %dx
    pop %eax
    pop %edx

    ret


read_from_port:
    push %edx
    mov 8(%esp), %edx
    in  %dx, %al
    pop %edx
    ret