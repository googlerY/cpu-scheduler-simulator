CC = gcc
CFLAGS = -Wall -Wextra

# 실시간 특화 알고리즘
ALGO1_SRC = src/algo1_realtime/main.c src/algo1_realtime/queue.c src/algo1_realtime/scheduler.c
ALGO1_OBJ = $(ALGO1_SRC:.c=.o)
ALGO1_BIN = scheduler_realtime.out

all: algo1

algo1: $(ALGO1_BIN)

$(ALGO1_BIN): $(ALGO1_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/algo1_realtime/*.o $(ALGO1_BIN)
