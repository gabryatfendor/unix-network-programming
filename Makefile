daytime:
	gcc -Iinc/ src/daytimeclient.c -o bin/daytime-client.out
clean:
	rm bin/*.out
