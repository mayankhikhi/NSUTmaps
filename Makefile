CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC = src/main.c src/graph.c src/queue.c src/stack.c src/shortest_path.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = nsut-campus-dsa-app.exe
TEST_EXEC = test_graph.exe
TEST_SRC = tests/test_graph.c src/graph.c src/queue.c src/stack.c src/shortest_path.c src/utils.c

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	$(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_EXEC)

clean:
	powershell -Command "Get-ChildItem -Recurse -Filter '*.o' | Remove-Item -Force -ErrorAction SilentlyContinue; Remove-Item -Force -ErrorAction SilentlyContinue '$(EXEC)', '$(TEST_EXEC)'"

.PHONY: all test clean