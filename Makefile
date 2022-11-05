daytime:
	gcc -Iinc/ src/daytimeclient.c -o bin/daytime-client.out
	gcc -Iinc/ src/daytimeclientipv6.c -o bin/daytime-client-ipv6.out
clean:
	rm bin/*.out
