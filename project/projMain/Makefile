# makfile configuration

CPU             	= msp430g2553

CFLAGS          	= -mmcu=${CPU} -Os -I../h

LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: snake.elf

#additional rules for files
snake.elf: ${COMMON_OBJECTS} snake.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load: snake.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
