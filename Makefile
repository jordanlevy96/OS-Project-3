DEVICE = /dev/tty.usbmodem1411

# compile:
# 	gcc main.c -o main

# test: compile
# 	./main

main: os.c serial.c
	avr-gcc -mmcu=atmega2560 -DF_CPU=16000000 -O2 -o os.elf os.c
	avr-objcopy -O ihex os.elf os.hex
	avr-size os.elf

#flash the Arduino with the program
program: os.hex
	#Mac
	avrdude -D -pm2560 -P $(DEVICE) -c wiring -F -u -U flash:w:os.hex

#remove build files
clean:
	rm -fr *.elf *.hex *.o

test: clean main program
	screen /dev/tty.usbmodem1411 115200