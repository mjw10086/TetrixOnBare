boot: boot.asm                                                                                                              
	as boot.asm -o boot.o 
	ld boot.o -s -Ttext=0x7c00 --oformat binary -o boot

io.o: io.asm
	as io.asm -o io.o --32

main: main.c lib.h io.o interrupt.h
	gcc main.c lib.c io.o interrupt.c -e main -nostartfiles -o main -m32

os_img: boot main
	dd if=boot of=os_img bs=512
	dd if=main of=os_img bs=512 seek=1 conv=sync
 
bochs_debug: os_img
	bochs -f bochsrc
        
clean:
	rm -f *.o boot main os_img
