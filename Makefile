plate: plate.o hash.o list.o plate.h
	gcc -g -o plate plate.o hash.o list.o
plate.o: plate.c plate.h
	gcc -g -c plate.c
hash.o: hash.c plate.h
	gcc -g -c hash.c
list.o: list.c plate.h
	gcc -g -c list.c
clean:
	rm list.o hash.o plate.o plate
