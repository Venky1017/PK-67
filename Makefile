CC=gcc
CFLAGS=-O3 -fopenmp
LDFLAGS=-lssl -lcrypto
TARGET=brute_force_ripemd

all: $(TARGET)

$(TARGET): brute_force_ripemd.c
	$(CC) $(CFLAGS) -o $(TARGET) brute_force_ripemd.c $(LDFLAGS)

clean:
	rm -f $(TARGET)
