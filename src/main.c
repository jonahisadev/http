#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "message.h"

#define CONNECTIONS 10

int main(int argc, char** argv) {
	// Variables
	unsigned short port;
	char buf[2048];
	struct sockaddr_in client;
	struct sockaddr_in server;
	int s;
	int ns;
	int c_len;

	// Get port
	port = (unsigned short) atoi(argv[1]);

	// Main Server
	s = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	bind(s, (struct sockaddr*)&server, sizeof(server));

	printf("Listening...\n");
	while (1) {
		// Listen
		listen(s, CONNECTIONS);

		// Accept connection
		c_len = sizeof(client);
		ns = accept(s, (struct sockaddr*)&client, (socklen_t*)&c_len);

		// Get info from client
		recv(ns, buf, sizeof(buf), 0);
		
		// Parse message into struct
		Message* msg = parse_message(buf);
		printf("Client request: "); print_message_header(msg);

		char* response;
		printf("file: %s\n", msg->header->file);
		if (!strcmp(msg->header->file, "/")) {
			response = "HTTP/1.1 200 OK\nServer: JonaHTTP/0.1\nContent-Type: text/html\n\nHello world!";
		} else {
			response = "HTTP/1.1 404 Not Found\nServer: JonaHTTP/0.1\n\n";
		}
		send(ns, response, strlen(response), 0);
		close(ns);

		delete_header(msg->header);
		free(msg);
	}

	close(s);

	printf("Server worked\n");

	return 0;
}