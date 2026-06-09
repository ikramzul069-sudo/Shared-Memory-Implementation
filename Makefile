COMPILER = gcc
CFLAGS = -fopenmp -std=c99 -O3

gccserial : main_serial.c
	$(COMPILER) $(CFLAGS) main_serial.c file_reader.c -o gccserial -lm

gccshared : main_shared.c
	$(COMPILER) $(CFLAGS) main_shared.c file_reader.c -o gccshared -lm