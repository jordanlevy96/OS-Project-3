#Jordan Levy and Chris Moranda

DEVICE = /dev/tty.usbmodem1411

main: os.c serial.c
	~/avr_files/bin/avr-gcc -mmcu=atmega2560 -DF_CPU=16000000 -O2 -o os.elf os.c
	~/avr_files/bin/avr-objcopy -O ihex os.elf os.hex
	~/avr_files/bin/avr-size os.elf

#flash the Arduino with the program
program: os.hex
	#Mac
	~/avr_files/bin/avrdude -D -pm2560 -P $(DEVICE) -c wiring -F -u -U flash:w:os.hex

#remove build files
clean:
	rm -fr *.elf *.hex *.o

test: clean main program
	screen /dev/tty.usbmodem1411 115200
