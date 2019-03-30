test: test.c filecompress.o
	gcc -fsanitize=address -o test test.c filecompress.o

filecompress.o: filecompress.c
	gcc -c filecompress.c

clean:
	rm -f filecompress.o
	rm -f test
