CFLAGS = -std=c99 -g -Wall -Wshadow -Wvla -Werror -Wunreachable-code
OBJS = main.o linked_list.o tile_game.o queue.o
HEADERS = linked_list.h tile_game.h
APP = hw10

.PHONY: clean

$(APP): $(OBJS)
	$(CC) $(CFLAGS) *.o -o $(APP)

clean:
	/bin/rm -rf *.o
	/bin/rm -rf $(APP)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

time_test: $(APP)
	@echo "test 1:  "
	time ./hw10 testcases/1.txt
	@echo "test 2:  "
	time ./hw10 testcases/2.txt
	@echo "test 3:  "
	time ./hw10 testcases/3.txt

