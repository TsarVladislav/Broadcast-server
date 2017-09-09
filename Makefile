CC = gcc
CFLAGS = -pedantic -Wextra -Wall
all: server client

server: server.o
	${CC} ${CFLAGS} $< -o server
server.o: server.c
	${CC} ${CFLAGS} -c $<
client: client.o
	${CC} ${CFLAGS} $< -o client
client.o: client.c
	${CC} ${CFLAGS} -c $<
clean:
	rm server client *.o
