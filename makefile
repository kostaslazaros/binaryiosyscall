names: write_system.o
	g++ -o names write_system.o -s

write_system.o: write_system.c
	gcc -Wall -O2 -c write_system.c -o write_system.o

clean:
	rm -f write_system.o names