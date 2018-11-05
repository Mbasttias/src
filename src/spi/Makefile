CC=gcc
CFLAGS=-I.
DEPS = VoSPI.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

VoSPI: main.o VoSPI.o 
	$(CC) -o VoSPI main.o VoSPI.o 
