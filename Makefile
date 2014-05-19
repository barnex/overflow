all:
	gcc -std=c99 -Wall -Werror -Wextra *.c -o overflow
	./overflow
