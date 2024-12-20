# Makefile to build and run the program

CC = gcc
CFLAGS = -Wall -O2 -fopenmp
LIBS = -lssl -lcrypto

TARGET = brute_force_ripemd
SRC = brute_force_ripemd.c

# Target to build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Target to clean the build artifacts
clean:
	rm -f $(TARGET)

# Target to run the program
run: $(TARGET)
	./$(TARGET)
