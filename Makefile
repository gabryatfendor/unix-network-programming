daytime:
	gcc -Iinc/ src/daytimeclient.c -o bin/daytime-client.out
	gcc -Iinc/ src/daytimeclientipv6.c -o bin/daytime-client-ipv6.out
	gcc -Iinc/ src/daytimeserver.c -o bin/daytime-server.out
	gcc -Iinc/ src/daytimeserveripv6.c -o bin/daytime-server-ipv6.out
	gcc -Iinc/ src/daytimeclientcounter.c -o bin/daytime-client-counter.out
	gcc -Iinc/ src/daytimeservercounter.c -o bin/daytime-server-counter.out
clean:
	rm bin/*.out
