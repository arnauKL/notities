TARGET = backfinder
CC = cc
CFLAGS = -Wall -Wextra -pedantic -std=c11

all: $(TARGET)

$(TARGET): backfinder.c
	$(CC) $(CFLAGS) backfinder.c -o $(TARGET)


.PHONY: run clean

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
