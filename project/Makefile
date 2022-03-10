# makfile configuration
COMMON_OBJECTS  = 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../demos/h -L/opt/ti/msp430_gcc/include

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: toy.elf 

#additional rules for files
toy.elf: ${COMMON_OBJECTS} buzzer.o main.o ../demos/lib/libTimer.a
	${CC} ${CFLAGS} -o $@ $^

load: toy.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf

buzzer.c: buzzer.h
main.c:


