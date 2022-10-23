boot: boot.asm                                                                                                              
	as boot.asm -o boot.o 
	ld boot.o -s -Ttext=0x7c00 --oformat binary -o boot
 
os_img: boot
	dd if=boot of=os_img bs=512
 
bochs_debug: os_img
	bochs -f bochsrc
        
clean:
	rm -f *.o boot os_img
