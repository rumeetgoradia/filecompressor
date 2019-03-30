all: fileCompressor.c fcdatastructs.o
	gcc -fsanitize=address -o fileCompressor fileCompressor.c fcdatastructs.o

fcdatastructs.o: fcdatastructs.c
	gcc -c fcdatastructs.c

clean:
	rm -f fcdatastructs.o
	rm -f fileCompressor
	rm -f HuffmanCodebook.hcz
