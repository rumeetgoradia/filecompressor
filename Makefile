all: fileCompressor.c fcompress.o
	gcc -fsanitize=address -o fileCompressor fileCompressor.c fcompress.o

fcompress.o: fcompress.c
	gcc -c fcompress.c

clean:
	rm -f fcompress.o
	rm -f fileCompressor
	rm -f HuffmanCodebook.hcz
