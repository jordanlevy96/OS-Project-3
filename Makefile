#Jordan Levy and Chris Moranda

DEVICE = /dev/tty.usbmodem1421

main: main.c os.c serial.c
	~/avr_files/bin/avr-gcc -mmcu=atmega2560 -DF_CPU=16000000 -O2 -o main.elf main.c
	~/avr_files/bin/avr-objcopy -O ihex main.elf main.hex
	~/avr_files/bin/avr-size main.elf

#flash the Arduino with the program
program: main.hex
	#Mac
	~/avr_files/bin/avrdude -D -pm2560 -P $(DEVICE) -c wiring -F -u -U flash:w:main.hex

#remove build files
clean:
	rm -fr *.elf *.hex *.o

test: clean main program
	screen /dev/tty.usbmodem1421 115200
