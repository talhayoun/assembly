assembler: assembler.o macro.o node.o util.o
	gcc -ansi -Wall -pedantic assembler.o address.o second_process.o first_process.o pre-process.o macro.o node.o util.o -o run

assembler.o: second_process.o first_process.o address.o pre-process.o macro.o node.o  ./headers/linked-list.h ./headers/util.h ./headers/data.h ./headers/global.h ./headers/pre-process.h ./headers/first_process.h ./headers/second_process.h ./headers/type.h
	gcc -c -ansi -Wall -pedantic ./assembler/assembler.c -o assembler.o

address.o: ./headers/global.h
	gcc -c -ansi -Wall -pedantic ./address/address.c -o address.o

first_process.o:  ./headers/util.h ./headers/global.h ./headers/first_process.h ./headers/type.h ./headers/address.h
	gcc -c -ansi -Wall -pedantic ./assembler/first_process.c -o first_process.o

second_process.o:  ./headers/type.h ./headers/second_process.h
	gcc -c -ansi -Wall -pedantic ./assembler/second_process.c -o second_process.o

macro.o: node.o ./headers/linked-list.h ./headers/util.h ./headers/data.h ./headers/global.h ./headers/type.h
	gcc -c -ansi -Wall -pedantic ./macros/macro.c -o macro.o

pre-process.o: ./headers/macro.h
	gcc -c -ansi -Wall -pedantic ./macros/pre-process.c -o pre-process.o

node.o: ./headers/linked-list.h
	gcc -c -ansi -Wall -pedantic ./linked-list/node.c -o node.o

util.o:
	gcc -c -ansi -Wall -pedantic ./utils/util.c -o util.o
	