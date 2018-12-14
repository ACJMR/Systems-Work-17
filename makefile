control: control.o
	gcc -o control control.o
main: main.o
	gcc -o main main.o
control.o: control.c
	gcc -c control.c
main.o: main.c
	gcc -c main.c
clean:
	rm -f *out *~ *o
