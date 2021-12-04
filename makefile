default: main date compte files transaction link
	
main: sources/main.c
	gcc -Wall -c sources/main.c -o binaries/main.o

date: sources/date.c
	gcc -Wall -c sources/date.c -o binaries/date.o

compte: sources/compte.c
	gcc -Wall -c sources/compte.c -o binaries/compte.o

files: sources/files_utils.c
	gcc -Wall -c sources/files_utils.c -o binaries/files.o

transaction: sources/transactions.c
	gcc -Wall -c sources/transactions.c -o binaries/transactions.o

link: binaries/main.o binaries/date.o binaries/compte.o binaries/files.o
	gcc -o main binaries/main.o binaries/date.o binaries/compte.o binaries/files.o binaries/transactions.o