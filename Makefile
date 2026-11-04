CC     = gcc
CFLAGS = -Wall -Wextra -O2 -pthread

TARGET = sync_car
OBJS   = sync_car.o sync_car-runner.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

sync_car.o: sync_car.c sync_car.h
	$(CC) $(CFLAGS) -c sync_car.c -o $@

sync_car-runner.o: sync_car-runner.c sync_car.h
	$(CC) $(CFLAGS) -c sync_car-runner.c -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all test clean