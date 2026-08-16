CC = gcc
CFLAGS = -Wall -Wextra

# 실시간 특화 알고리즘
ALGO1_SRC = src/algo1_realtime/main.c src/algo1_realtime/queue.c src/algo1_realtime/scheduler.c
ALGO1_OBJ = $(ALGO1_SRC:.c=.o)
ALGO1_BIN = scheduler_realtime.out

# 혼합 알고리즘 (SRTF + RR + 비선점 우선순위 + FCFS)
ALGO2_SRC = src/algo2_mixed/main.c src/algo2_mixed/queue.c src/algo2_mixed/scheduler.c
ALGO2_OBJ = $(ALGO2_SRC:.c=.o)
ALGO2_BIN = scheduler_mixed.out

all: algo1 algo2

algo1: $(ALGO1_BIN)

algo2: $(ALGO2_BIN)

$(ALGO1_BIN): $(ALGO1_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(ALGO2_BIN): $(ALGO2_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/algo1_realtime/*.o src/algo2_mixed/*.o $(ALGO1_BIN) $(ALGO2_BIN)
