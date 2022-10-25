boot: boot.asm                                                                                                              
	as boot.asm -o boot.o 
	ld boot.o -s -Ttext=0x7c00 --oformat binary -o boot

main: main.c
	gcc main.c -e main -nostartfiles -o main -m32

os_img: boot main
	dd if=boot of=os_img bs=512
	dd if=main of=os_img bs=512 seek=1 conv=sync
 
bochs_debug: os_img
	bochs -f bochsrc
        
clean:
	rm -f *.o boot main os_img
