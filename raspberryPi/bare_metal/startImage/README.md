# startImage

Take a sd card format with FAT32 and put these files on it.

Tested on:
	Raspberry Pi 2 Model B V1.1 (Broadcom BCM2836, ARMv7 Cortex-A7, 4 cores)
	
not working on:
	Raspberry Pi Model B+ V1.2 (Broadcom BCM2835, ARM11, 1 core)
	
Then insert the sd card into the Raspberry, connect via JTAG and load your programm.
My adapter is segger j-link.


bootcode.bin, start.elf : boot files for gpu
config.txt : file to tell the gpu how to start the cpu
kernel.img: memory shadow which ist loaded into ram (gpu does this)
