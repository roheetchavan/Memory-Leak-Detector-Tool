all:
	gcc -g -c mldt.c -o mldt.o 	
	gcc -g -c app.c -o app.o

	gcc -g -o mldt_demo.exe mldt.o app.o
clean:
	rm -rf mldt.o app.o mldt_demo.exe	
