compile :
	reset
	gcc -Wall -Wextra ./config/*.c ./model/*.c ./controller/*.c ./database/*.c *.c ./view/*.c ./utils/*.c `mysql_config --cflags --include --libs` -o ./client.out

run :
	make
	reset
	./client.out
