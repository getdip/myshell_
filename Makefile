all: myshell

myshell: main.o check_cmd.o echo.o ext_cmd.o int_cmd.o jobs.o pipes.o
	@gcc check_cmd.o echo.o ext_cmd.o int_cmd.o jobs.o main.o pipes.o -o myshell

main.o: main.c main.h
	@gcc -c main.c

check_cmd.o: check_cmd.c main.h
	@gcc -c check_cmd.c

echo.o: echo.c main.h
	@gcc -c echo.c

ext_cmd.o: ext_cmd.c main.h
	@gcc -c ext_cmd.c

int_cmd.o: int_cmd.c main.h
	@gcc -c int_cmd.c

jobs.o: jobs.c main.h
	@gcc -c jobs.c

pipes.o: pipes.c main.h
	@gcc -c pipes.c

clean:
	@rm -rf *.o *.out myshell

run:
	@./myshell