compile :
	reset
	gcc -Wall -Wextra ./config/*.c ./controller/*.c ./database/*.c ./main/*.c ./view/*.c ./utils/*.c `mysql_config --cflags --include --libs` -o ./client.out

run :
	make
	reset
	./client.out
