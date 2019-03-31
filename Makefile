all: fileCompressor.c fcdatastructs.o tokenizer.o
	gcc -fsanitize=address -o fileCompressor fileCompressor.c fcdatastructs.o tokenizer.o

fcdatastructs.o: fcdatastructs.c
	gcc -c fcdatastructs.c

tokenizer.o: tokenizer.c
	gcc -c tokenizer.c

clean:
	rm -f fcdatastructs.o
	rm -f tokenizer.o
	rm -f fileCompressor
	rm -f HuffmanCodebook
	rm -f *.hcz
