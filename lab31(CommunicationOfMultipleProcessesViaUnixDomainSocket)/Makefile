all: server.out client.out

server.out: server.c
	cc server.c -lsocket -lnsl -o server.out
client.out: client.c
	cc client.c -lsocket -lnsl -o client.out
clean:
	rm -rf server.out client.out
