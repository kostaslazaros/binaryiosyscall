catalog: catalog.o
	g++ -o catalog catalog.o -s

write_system.o: catalog.c
	gcc -Wall -O2 -c catalog.c -o catalog.o

clean:
	rm -f catalog.o catalog data.bin pos.bin a.out
