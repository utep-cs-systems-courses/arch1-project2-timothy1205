# makfile configuration
OBJS 		= main.o input.o output.o morse.o toy.o
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../demos/h -L/opt/ti/msp430_gcc/include

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: toy.elf 

#additional rules for files
toy.elf: ${OBJS} ../demos/lib/libTimer.a
	${CC} ${CFLAGS} -o $@ $^

load: toy.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf

input.c: input.h toy.h
output.c: output.h morse.h toy.h
morse.c: morse.h
toy.c: toy.h output.h
main.c: input.h output.h toy.h


